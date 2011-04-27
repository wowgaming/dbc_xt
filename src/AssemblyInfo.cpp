#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// Le informazioni generali relative a un assembly sono controllate dal seguente
// insieme di attributi. Per modificare le informazioni associate a un assembly
// occorre quindi modificare i valori di questi attributi.
//
[assembly:AssemblyTitleAttribute("csvdbc")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("csvdbc")];
[assembly:AssemblyCopyrightAttribute("Copyright (c)  2011")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// Le informazioni sulla versione di un assembly sono costituite dai seguenti quattro valori:
//
//      Numero di versione principale
//      Numero di versione secondario
//      Numero build
//      Revisione
//
// È possibile specificare tutti i valori o impostare come predefiniti i valori Revisione e Numero build
// utilizzando l'asterisco (*) come descritto di seguito:

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];
