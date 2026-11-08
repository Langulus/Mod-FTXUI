///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Thing.hpp>
#include <Langulus/CppAPI/UI.hpp>
#include <Langulus/Time.hpp>
#include <Langulus/Framerate.hpp>
#include <thread>

using namespace Langulus;
using Things::Thing;

int main(int, char**) {
   Framerate<60> fps;

   // Create root entity                                                
   auto root = Thing::Root<false>("FTXUI");

   // Create user interface                                             
   root.CreatePart<Things::UISystem>();

   while (true) {
      // Update until quit                                              
      if (not root.Update(16ms))
         break;

      fps.Tick();
   }

   return 0;
}
