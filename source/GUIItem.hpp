///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Producible.hpp>


///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIItem : Things::UIUnit, ProducedFrom<GUISystem> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = GUISystem;
   using CTTI_Bases     = Things::UIUnit;

public:
   GUIItem(GUISystem*, const Many&);

   virtual void Update(Time) {}
   void Refresh();
};

