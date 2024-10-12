///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Main.hpp"
#include <Langulus/UI.hpp>
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("GUI creation", "[gui]") {
   static Allocator::State memoryState;
   
   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FTXUI");

         WHEN("The GUI system is created via abstractions") {
            auto gui = root.CreateUnit<A::UISystem>();

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<A::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }

      /*#if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The GUI system is created via tokens") {
            auto gui = root.CreateUnitToken("GUISystem");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<A::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }

         WHEN("The GUI system is created via tokens") {
            auto gui = root.CreateUnitToken("UISystem");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gui.GetCount() == 1);
            REQUIRE(gui.CastsTo<A::UISystem>(1));
            REQUIRE(gui.IsSparse());
            REQUIRE(root.GetUnits().GetCount() == 1);
         }
      #endif*/

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

