///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUI.hpp"
#include <Langulus/Verbs/Interact.hpp>
#include <Langulus/Entity/Event.hpp>
#include <Langulus/Math/Vector.hpp>

LANGULUS_DEFINE_MODULE(
   GUI, 9, "FTXUI",
   "GUI generator and simulator, using FTXUI as backend", "",
   GUI, GUISystem, GUIItem, GUIEditor
)

using namespace ftxui;


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
GUI::GUI(Runtime* runtime, const Many&)
   : Resolvable {this}
   , A::Module  {runtime} {
   VERBOSE_GUI("Initializing...");
   VERBOSE_GUI("Initialized");
}

/// First stage of destruction                                                
void GUI::Teardown() {
   mSystems.Teardown();
}

/// Module update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the UI requested exit                                  
bool GUI::Update(Time deltaTime) {
   for (auto& system : mSystems)
      system.Update(deltaTime);
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(this, verb);
}