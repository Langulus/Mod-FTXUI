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
struct GUIPart : Things::UIPart, ProducedFrom<GUISystem> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = GUISystem;
   using CTTI_Bases     = Things::UIPart;

public:
   GUIPart(GUISystem*, Many const&);

   virtual void Update(Time) {}
   void Refresh();
};

