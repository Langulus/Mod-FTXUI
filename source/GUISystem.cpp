///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUISystem.hpp"
#include "GUI.hpp"


/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, const Neat& descriptor)
   : A::UI::System {MetaOf<GUISystem>(), descriptor}
   , ProducedFrom {producer, descriptor}
   , mItems {this} {

}

/// GUI system destruction                                                    
GUISystem::~GUISystem() {

}

/// Produce GUI elements and fonts                                            
///   @param verb - creation verb to satisfy                                  
void GUISystem::Create(Verb& verb) {
   mItems.Create(verb);
}

void GUISystem::Draw(Verb&) {

}

/// React on environmental change                                             
void GUISystem::Refresh() {

}