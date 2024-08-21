#include "../../include/core/bt.h"

Behavior::Behavior() : status(BTStatus::INVALID) {}
Behavior::~Behavior(){}

BTStatus Behavior::tick(){
    if(this->status != BTStatus::RUNNING){
        onInitialize();
    }

    this->status = update();

    if(this->status != BTStatus::RUNNING){
        onTerminate(this->status);
    }

    return this->status;
}

BTStatus Behavior::getStatus(){
    return this->status;
}

void Behavior::onInitialize(){}
void Behavior::onTerminate(BTStatus){}

void Behavior::reset(){
    this->status = BTStatus::INVALID;
}

void Behavior::abort(){
    onTerminate(BTStatus::ABORTED);
    this->status = BTStatus::ABORTED;
}

bool Behavior::isTerminated() const{
    return this->status == BTStatus::SUCCESS || this->status == BTStatus::FAILURE;
}

bool Behavior::isRunning() const{
    return this->status == BTStatus::RUNNING;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Decorator::Decorator(Behavior* child) : child(child){}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Repeat::Repeat(Behavior* child) : Decorator(child) {}

void Repeat::setLimit(Uint32 limit){
    this->limit = limit;
}
void Repeat::onInitialize(){
    this->counter = 0;
}

BTStatus Repeat::update(){
    while(true) {
        this->child->tick();
        if(this->child->getStatus() == BTStatus::RUNNING) break;
        if(this->child->getStatus() == BTStatus::FAILURE) return BTStatus::FAILURE;
        if(++this->counter == this->limit) return BTStatus::SUCCESS;
        this->child->reset();
    }

    return BTStatus::RUNNING;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void Composite::addChild(Behavior* child){
    this->children.push_back(child);
}

void Composite::removeChild(Behavior*){}

void Composite::clearChildren(){}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Sequence::~Sequence(){}

void Sequence::onInitialize() {
    this->currentChild = this->children.begin();
}

BTStatus Sequence::update() {

    // run until child node returns
    while(true){
        BTStatus partialStatus = (*this->currentChild)->tick();

        // if child does not have success status, return 
        if(partialStatus != BTStatus::SUCCESS){
            return partialStatus;
        }

        // return success if you have reached end of sequence
        if(++this->currentChild == this->children.end()){
            return BTStatus::SUCCESS;
        }
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Selector::~Selector(){}
void Selector::onInitialize(){
    this->current = this->children.begin();
}

BTStatus Selector::update(){

    // run until child node returns 
    while(true){

        BTStatus partialStatus = (*this->current)->tick();

        // if child does not have failure status, return 
        if(partialStatus != BTStatus::FAILURE){
            return partialStatus;
        }

        // returns failure if you have reached end of selector
        if(++this->current == this->children.end()){
            return BTStatus::FAILURE;
        }
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Parallel::~Parallel(){}


Parallel::Parallel(Policy success, Policy failure) 
: successPolicy(success), 
failurePolicy(failure){

}


BTStatus Parallel::update(){
    Uint32 successCount = 0;
    Uint32 failureCount = 0;

    for(std::vector<Behavior*>::iterator it = this->children.begin(); it != this->children.end(); it++){
        Behavior& b = **it;
        if(!b.isTerminated()){
            b.tick();
        }

        if(b.getStatus() == BTStatus::SUCCESS){
            successCount++;
            if(this->successPolicy == Policy::RequireOne){
                return BTStatus::FAILURE;
            }
        }

        if(b.getStatus() == BTStatus::FAILURE){
            failureCount++;
            if(this->failurePolicy == Policy::RequireOne){
                return BTStatus::FAILURE;
            }
        }
    }

    if(this->failurePolicy == Policy::RequireAll && failureCount == this->children.size()){
        return BTStatus::FAILURE;
    }

    if(this->successPolicy == Policy::RequireAll && successCount == this->children.size()){
        return BTStatus::SUCCESS;
    }

    return BTStatus::RUNNING;
}

void Parallel::onTerminate(BTStatus){
    for(std::vector<Behavior*>::iterator it = this->children.begin(); it != this->children.end(); it++){
        Behavior& b = **it;
        if(b.isRunning()){
            b.abort();
        }
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Monitor::Monitor() : Parallel(Parallel::RequireOne, Parallel::RequireOne){}

void Monitor::addCondition(Behavior* condition){
    this->children.insert(this->children.begin(), condition);
}

void Monitor::addAction(Behavior* action){
    this->children.push_back(action);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void ActiveSelector::onInitialize(){
    this->current = this->children.end();
}

BTStatus ActiveSelector::update(){
    std::vector<Behavior*>::iterator previous = this->current;
    Selector::onInitialize();
    BTStatus result = Selector::update();

    if(previous != this->children.end() && this->current != previous){
        (*previous)->onTerminate(BTStatus::ABORTED);
    }

    return result;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

