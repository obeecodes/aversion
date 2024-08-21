#pragma once

#include <vector>
#include <SDL2/SDL_stdinc.h>

enum BTStatus {
    INVALID,
    SUCCESS,
    RUNNING,
    FAILURE,
    ABORTED,

};

class Behavior {
public:
    Behavior();
    ~Behavior();
    BTStatus tick();
    BTStatus getStatus();
    void reset();
    void abort();
    bool isTerminated() const;
    bool isRunning() const;

    virtual void onInitialize();
    virtual BTStatus update() = 0;
    virtual void onTerminate(BTStatus);

private:
    BTStatus status;

};

class Composite : public Behavior {
public:
    void addChild(Behavior*);
    void removeChild(Behavior*);
    void clearChildren();

protected:
    std::vector<Behavior*> children;

private:

};

class Sequence : public Composite {
public:

protected:
    ~Sequence();
    virtual void onInitialize() override;
    virtual BTStatus update() override;
    std::vector<Behavior*>::iterator currentChild;

private:

};

class Selector : public Composite {
protected: 
    ~Selector();
    virtual void onInitialize();
    virtual BTStatus update();
    std::vector<Behavior*>::iterator current;

};

class Parallel : public Composite {
public: 

    ~Parallel();
    enum Policy {
        RequireOne,
        RequireAll,
    };
    Parallel(Policy success, Policy failure);

protected:
    Policy successPolicy;
    Policy failurePolicy;
    virtual BTStatus update();
    virtual void onTerminate(BTStatus);
};

class Monitor : public Parallel {
public:

    Monitor();

    void addCondition(Behavior* condition);

    void addAction(Behavior* action);
};

class ActiveSelector : public Selector {
protected:
    virtual void onInitialize();
    virtual BTStatus update();

};

class Decorator : public Behavior {
public:
    Decorator(Behavior* child);

protected:
    Behavior* child;

private:

};

class Repeat : public Decorator {
public: 
    Repeat(Behavior* child);
    void setLimit(Uint32 limit);
    void onInitialize();
    BTStatus update();
protected:
    Uint32 limit;
    Uint32 counter;

};