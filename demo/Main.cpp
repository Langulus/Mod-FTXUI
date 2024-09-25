///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus.hpp>
#include <Langulus/UI.hpp>
#include <Flow/Time.hpp>
#include <thread>

LANGULUS_RTTI_BOUNDARY(RTTI::MainBoundary)
using namespace Langulus;


int main(int, char**) {
   Framerate<60> fps;

   // Create root entity                                                
   auto root = Thing::Root<false>("FTXUI");

   // Create user interface                                             
   root.CreateUnit<A::UISystem>();

   while (true) {
      // Update until quit                                              
      if (not root.Update(16ms))
         break;

      fps.Tick();
   }

   return 0;
}
