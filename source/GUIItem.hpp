///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Langulus/Flow/Producible.hpp>


///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIItem final : A::UIUnit, ProducedFrom<GUISystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUISystem;
   LANGULUS_BASES(A::UIUnit);

public:
   GUIItem(GUISystem*, const Many&);

   virtual void Update(Time) {}
   void Refresh();
};

