///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUIItem.hpp"
#include "GUI.hpp"


/// GUI item construction                                                     
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the item               
GUIItem::GUIItem(GUISystem* producer, Describe descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_GUI("Initializing...");
   Couple(descriptor);
   VERBOSE_GUI("Initialized");
}

/// React on environmental change                                             
void GUIItem::Refresh() {

}

