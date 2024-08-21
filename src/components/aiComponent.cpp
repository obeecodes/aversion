#include "../../include/components/aiComponent.h"

AIComponent::AIComponent(){
  this->script = "";
}

AIComponent::AIComponent(std::string script)
  : script(script){}

