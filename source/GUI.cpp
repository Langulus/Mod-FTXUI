///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUI.hpp"

LANGULUS_DEFINE_MODULE(
   GUI, 9, "FTXUI",
   "GUI generator and simulator, using FTXUI as backend", "",
   GUI, GUISystem, GUIItem
)

using namespace ftxui;


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
GUI::GUI(Runtime* runtime, const Neat&)
   : A::UI::Module {MetaOf<GUI>(), runtime}
   , mSystems {this}
   , mScreen {ScreenInteractive::Fullscreen()}
   , mMain {Container::Vertical({})} {
   Logger::Verbose(Self(), "Initializing...");

   // Create the main loop                                              
   try { mLoop = new Loop(&mScreen, mMain); }
   catch (const std::exception& e) {
      Logger::Error(Self(), "Unable to create FTXUI main loop: ", e.what());
      throw;
   }

   Logger::Verbose(Self(), "Initialized");
}

/// Shutdown the module                                                       
GUI::~GUI() {
   if (mLoop)
      delete mLoop;
}

/// Module update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the UI requested exit                                  
bool GUI::Update(Time deltaTime) {
   if (mLoop and mLoop->HasQuitted())
      return false;

   // Update all GUI systems                                            
   for (auto& system : mSystems)
      system.Update(deltaTime);

   // Yield FTXUI                                                       
   mLoop->RunOnce();
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(verb);
}

/// Get the main FTXUI container                                              
///   @return a reference to the container                                    
ftxui::Component& GUI::GetContainer() noexcept {
   return mMain;
}
