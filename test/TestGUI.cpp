///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/UI.hpp>
#include <Langulus/Testing.hpp>
#include <Langulus/Allocator.hpp>

using namespace Langulus;
using Things::Thing;


SCENARIO("GUI creation") {
   static MemoryState memoryState;
   
   GIVEN("Init and shutdown cycles") {
      for (int repeat = 0; repeat != 10; ++repeat) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FTXUI");

         WHEN("The GUI system is created via abstractions") {
            auto gui = root.CreatePart<Things::UISystem>();

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<Things::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetParts().GetCount() == 1);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The GUI system is created via tokens") {
            auto gui = root.CreatePartToken("GUISystem");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<Things::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetParts().GetCount() == 1);
         }

         WHEN("The GUI system is created via tokens") {
            auto gui = root.CreatePartToken("UISystem");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<Things::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetParts().GetCount() == 1);
         }
      #endif

         REQUIRE(memoryState.Assert());
         REQUIRE_FALSE(Allocator::CollectGarbage());
      }
   }
}

