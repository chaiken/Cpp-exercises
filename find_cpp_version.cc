// http://www.cplusplus.com/forum/general/90675/
#include <iostream>

// 'g++ --dumpspecs' just says
///////////////////////////////////////////////////////////////////////////////
//	*cpp:								     //
//	%{posix:-D_POSIX_SOURCE} %{pthread:-D_REENTRANT}		     //
//									     //
//	*cpp_options:							     //
//	%(cpp_unique_options) %1 %{m*} %{std*&ansi&trigraphs} %{W*&pedantic*}
//%{w} %{f*} %{g*:%//
//	{!g0:%{g*} %{!fno-working-directory:-fworking-directory}}} %{O*}
//%{undef} %{save-temps*//
//	:-fpch-preprocess}						     //
//									     //
//	*cpp_debug_options:						     //
//	%{d*}								     //
//									     //
//	*cpp_unique_options:						     //
//	%{!Q:-quiet} %{nostdinc*} %{C} %{CC} %{v} %{I*&F*} %{P} %I %{MD:-MD
//%{!o:%b.d}%{o*:%.d%//
//	*}} %{MMD:-MMD %{!o:%b.d}%{o*:%.d%*}} %{M} %{MM} %{MF*} %{MG} %{MP}
//%{MQ*} %{MT*} %{!E://
//	%{!M:%{!MM:%{!MT:%{!MQ:%{MD|MMD:%{o*:-MQ %*}}}}}}} %{remap}
//%{g3|ggdb3|gstabs3|gcoff3|g//
//	xcoff3|gvms3:-dD} %{!iplugindir*:%{fplugin*:%:find-plugindir()}} %{H} %C
//%{D*&U*&A*} %{//
//	i*} %Z %i %{E|M|MM:%W{o*}}					     //
//									     //
///////////////////////////////////////////////////////////////////////////////

int main(void) {
  if (__cplusplus == 201103L)
    std::cout << "C++11\n";
  else if (__cplusplus == 19971L)
    std::cout << "C++98\n";
  else
    std::cout << "pre-standard C++\n";
}
