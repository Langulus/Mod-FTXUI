///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include <Entity/External.hpp>

using namespace Langulus;
using namespace Langulus::Anyness;
using namespace Langulus::Entity;

LANGULUS_RTTI_BOUNDARY("MAIN")

int main(int argc, char* argv[]) {
   // Create root entity                                                
   Thing root;
   root.SetName("ROOT");
   root.CreateRuntime();
   root.LoadMod("FTXUI");

   // Create user interface                                             
   auto gui = root.CreateUnit<A::UI::System>();

   // Update until quit                                                 
   while (root.Update(Time::zero()));
   return 0;
}
