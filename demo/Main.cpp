///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include <Langulus.hpp>
#include <Langulus/UI.hpp>
#include <Flow/Time.hpp>
#include <thread>

using namespace Langulus;

LANGULUS_RTTI_BOUNDARY(RTTI::MainBoundary)


int main(int, char**) {
   // Create root entity                                                
   Thing root;
   root.SetName("ROOT");
   root.CreateRuntime();
   root.LoadMod("FTXUI");

   // Create user interface                                             
   auto gui = root.CreateUnit<A::UI::System>();

   do {
      // Update until quit                                              
      constexpr auto dt = 16ms;
      const auto nextRefresh = SteadyClock::Now() + dt;
      if (not root.Update(dt))
         break;
      std::this_thread::sleep_until(nextRefresh);
   }
   while (true);
   return 0;
}
