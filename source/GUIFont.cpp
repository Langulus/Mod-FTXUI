///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUIItem.hpp"
#include "GUI.hpp"
#include <Math/Color.hpp>
#include <Flow/Verbs/Interpret.hpp>


/// GUI item construction                                                     
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the font               
GUIFont::GUIFont(GUISystem* producer, const Neat& descriptor)
   : A::UI::Unit {MetaOf<GUIFont>(), descriptor}
   , ProducedFrom {producer, descriptor} {

}