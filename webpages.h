#ifndef header_h
#define header_h

String file1=
"<h1 style=\"color: #5e9ca0;\">This is the HOME page!&nbsp;</h1>\r\n"
"<p>Go to the&nbsp;<a href=\"lightstatus\" target=\"_self\">Light Status Page</a></p>\r\n"
"<p>Go to the&nbsp;<a href=\"timeoutsetting\" target=\"_self\">Light Timeout Option page</a></p>\r\n"
"<p><img src=\"https://tuts.codingo.me/wp-content/uploads/2016/05/ESP-12.jpg\" alt=\"Mighty Mighty Chip!\" width=\"308\" height=\"231\" /></p>\r\n";

String file2a=
"<h1><span style=\"color: #000000;\">Light Status Page</span></h1>\r\n";
String file2b=
"<p>Nuber of times lights have been switched:&nbsp;</p>\r\n"
"<table>\r\n"
"<tbody>\r\n"
"<tr>\r\n";
String file2c=
"</tr>\r\n"
"</tbody>\r\n"
"</table>\r\n"
"<p>Go to the&nbsp;<a href=\"timeoutsetting\" target=\"_self\">Light Timeout Option page</a></p>\r\n"
"<p><br /><img src=\"https://tuts.codingo.me/wp-content/uploads/2016/05/ESP-12.jpg\" alt=\"Mighty Mighty Chip!\" width=\"308\" height=\"231\" /></p>\r\n"
"Timeout counter in 0.5 second interval:  ";


String file3=
"<h1><span style=\"color: #000000;\">Lighting timer delay page</span></h1>\r\n"
"<p>Enter the amount of time for the lights to stay on after motion being detected:&nbsp;</p>\r\n"
"<body>"
"</p><form method='get' action='setting'><label>Delay time in minutes: </label><input name='Time' length=32><input type='submit'></form>"
"<p>Click the Submit button to confirm time and update motion sensor.</p>"
"</body>"
"</html>";

String file4=
"<h1>Success: Saved new timeout. Return to status to check lights"
"<p>Go to the <a href=\"lightstatus\" target=\"_self\">Light Status Page</a></p>\r\n";

#endif
