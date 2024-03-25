///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/UI.hpp>
#include <Langulus/Platform.hpp>

using namespace Langulus;

struct GUI;
struct GUISystem;
struct GUIItem;

#define VERBOSE_GUI(...)      Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_GUI_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})

/// Include FTXUI                                                             
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>