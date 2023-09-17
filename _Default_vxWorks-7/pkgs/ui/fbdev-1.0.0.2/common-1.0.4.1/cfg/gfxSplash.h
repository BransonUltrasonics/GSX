/* gfxSplash.h - Splash screen */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
24jan14,mgc  Modified for VxWorks 7 release
*/

#ifndef __INC_gfxSplash_h
#define __INC_gfxSplash_h

#include <stdio.h>
unsigned int wrSplashInflatedSize = 273078;
#if (FB_SPLASH_WR != TRUE)
unsigned char wrSplashDeflatedData[] = {0x1d, 0x37, 0x53};
#else

unsigned char wrSplashDeflatedData[] = {8, 120 ,156 ,237 ,221 ,201 ,142 ,227 ,200 ,181 ,128 ,225 ,244 ,240 ,48 ,134 ,159 ,194 ,48 ,18 ,41 ,109 ,140 ,90 ,249 ,13 ,132 ,74 ,238 ,106 ,83 ,104 ,245 ,174 ,107 ,33 ,164 ,180 ,227 ,219 ,120 ,221 ,15 ,213 ,232 ,202 ,157 ,156 ,74 ,77 ,28 ,78 ,4 ,131 ,140 ,19 ,17 ,135 ,212 ,255 ,121 ,194 ,149 ,148 ,29 ,196 ,127 ,131 ,226 ,32 ,138 ,250 ,247 ,127 ,254 ,247 ,207 ,191 ,63 ,157 ,252 ,242 ,241 ,159 ,127 ,124 ,252 ,231 ,199 ,95 ,159 ,158 ,254 ,251 ,183 ,167 ,167 ,191 ,60 ,157 ,31 ,255 ,215 ,229 ,249 ,190 ,31 ,159 ,255 ,126 ,250 ,241 ,227 ,252 ,63 ,167 ,127 ,157 ,30 ,250 ,248 ,175 ,223 ,127 ,255 ,253 ,227 ,145 ,227 ,231 ,191 ,159 ,142 ,199 ,243 ,255 ,156 ,254 ,117 ,122 ,232 ,227 ,191 ,14 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,40 ,108 ,59 ,85 ,233 ,5 ,55 ,226 ,121 ,170 ,95 ,75 ,47 ,185 ,13 ,147 ,251 ,85 ,165 ,151 ,220 ,6 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,185 ,245 ,232 ,158 ,223 ,251 ,126 ,125 ,226 ,85 ,126 ,184 ,42 ,189 ,228 ,54 ,220 ,250 ,117 ,159 ,216 ,201 ,161 ,28 ,15 ,63 ,44 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,28 ,250 ,197 ,161 ,95 ,156 ,37 ,245 ,59 ,78 ,246 ,62 ,121 ,76 ,250 ,209 ,239 ,138 ,126 ,113 ,232 ,23 ,135 ,126 ,113 ,232 ,23 ,135 ,126 ,113 ,232 ,23 ,135 ,126 ,113 ,232 ,23 ,199 ,84 ,191 ,250 ,250 ,207 ,110 ,63 ,76 ,191 ,54 ,103 ,63 ,7 ,250 ,181 ,141 ,237 ,247 ,70 ,191 ,150 ,177 ,253 ,62 ,124 ,167 ,223 ,221 ,132 ,126 ,231 ,191 ,170 ,38 ,15 ,153 ,76 ,137 ,126 ,83 ,237 ,13 ,126 ,127 ,203 ,89 ,195 ,177 ,45 ,188 ,63 ,158 ,191 ,159 ,69 ,244 ,139 ,67 ,191 ,56 ,244 ,139 ,67 ,191 ,56 ,244 ,139 ,83 ,160 ,223 ,237 ,159 ,48 ,218 ,212 ,17 ,19 ,42 ,208 ,239 ,118 ,56 ,59 ,218 ,212 ,17 ,19 ,162 ,95 ,28 ,250 ,197 ,25 ,238 ,247 ,30 ,246 ,120 ,56 ,250 ,209 ,239 ,134 ,126 ,113 ,70 ,247 ,59 ,210 ,175 ,105 ,184 ,223 ,81 ,254 ,131 ,233 ,59 ,99 ,143 ,214 ,239 ,93 ,126 ,120 ,242 ,144 ,139 ,236 ,215 ,203 ,113 ,60 ,74 ,1 ,27 ,199 ,14 ,147 ,135 ,124 ,184 ,126 ,247 ,128 ,181 ,240 ,216 ,104 ,203 ,236 ,231 ,218 ,204 ,186 ,208 ,239 ,147 ,107 ,125 ,28 ,202 ,71 ,191 ,179 ,70 ,17 ,199 ,195 ,234 ,253 ,246 ,97 ,55 ,75 ,156 ,71 ,191 ,90 ,76 ,50 ,156 ,47 ,241 ,185 ,164 ,227 ,119 ,33 ,95 ,149 ,118 ,204 ,73 ,38 ,159 ,140 ,75 ,185 ,80 ,199 ,159 ,210 ,218 ,91 ,165 ,28 ,114 ,42 ,139 ,253 ,222 ,164 ,122 ,9 ,199 ,139 ,49 ,208 ,239 ,221 ,249 ,68 ,186 ,69 ,146 ,86 ,221 ,149 ,197 ,115 ,207 ,159 ,6 ,250 ,185 ,250 ,38 ,235 ,247 ,139 ,52 ,249 ,126 ,179 ,251 ,97 ,203 ,208 ,90 ,234 ,124 ,34 ,9 ,113 ,213 ,53 ,120 ,217 ,198 ,157 ,119 ,5 ,126 ,119 ,61 ,159 ,104 ,62 ,72 ,171 ,238 ,58 ,205 ,80 ,90 ,204 ,244 ,219 ,139 ,107 ,238 ,74 ,127 ,32 ,101 ,190 ,126 ,174 ,231 ,19 ,228 ,219 ,74 ,115 ,111 ,107 ,122 ,213 ,61 ,27 ,234 ,39 ,77 ,64 ,245 ,133 ,16 ,15 ,55 ,230 ,80 ,239 ,224 ,91 ,131 ,29 ,207 ,171 ,207 ,190 ,189 ,184 ,203 ,50 ,155 ,219 ,221 ,187 ,242 ,93 ,59 ,213 ,242 ,195 ,90 ,246 ,226 ,220 ,155 ,77 ,189 ,195 ,240 ,62 ,74 ,157 ,50 ,159 ,180 ,213 ,120 ,157 ,83 ,189 ,144 ,109 ,108 ,178 ,171 ,134 ,54 ,82 ,62 ,229 ,49 ,210 ,243 ,206 ,190 ,132 ,196 ,122 ,243 ,216 ,106 ,180 ,149 ,200 ,119 ,60 ,74 ,245 ,170 ,244 ,3 ,39 ,145 ,59 ,95 ,253 ,167 ,84 ,207 ,254 ,238 ,178 ,91 ,222 ,201 ,39 ,237 ,178 ,204 ,118 ,242 ,101 ,183 ,151 ,234 ,125 ,51 ,123 ,146 ,202 ,152 ,189 ,180 ,221 ,248 ,66 ,189 ,64 ,59 ,105 ,242 ,173 ,75 ,47 ,213 ,156 ,72 ,111 ,126 ,47 ,165 ,23 ,106 ,86 ,164 ,53 ,120 ,53 ,147 ,19 ,6 ,77 ,142 ,67 ,55 ,23 ,189 ,129 ,247 ,210 ,41 ,151 ,249 ,21 ,44 ,214 ,239 ,131 ,116 ,222 ,96 ,110 ,5 ,75 ,246 ,155 ,251 ,121 ,171 ,147 ,162 ,253 ,92 ,231 ,77 ,103 ,84 ,176 ,112 ,191 ,143 ,130 ,226 ,199 ,30 ,149 ,254 ,64 ,105 ,20 ,239 ,119 ,112 ,28 ,140 ,204 ,100 ,119 ,198 ,66 ,63 ,121 ,127 ,112 ,30 ,115 ,208 ,70 ,63 ,249 ,136 ,110 ,53 ,131 ,239 ,120 ,26 ,233 ,119 ,112 ,28 ,147 ,24 ,190 ,112 ,227 ,204 ,78 ,63 ,249 ,176 ,216 ,250 ,28 ,52 ,212 ,175 ,62 ,30 ,165 ,181 ,120 ,157 ,114 ,204 ,104 ,134 ,250 ,29 ,92 ,39 ,165 ,45 ,31 ,145 ,216 ,234 ,247 ,177 ,60 ,82 ,64 ,195 ,31 ,200 ,89 ,235 ,119 ,144 ,175 ,0 ,52 ,187 ,55 ,104 ,176 ,159 ,188 ,63 ,253 ,117 ,46 ,39 ,22 ,196 ,84 ,89 ,251 ,125 ,140 ,38 ,93 ,141 ,181 ,154 ,71 ,192 ,228 ,253 ,194 ,190 ,255 ,33 ,109 ,138 ,53 ,70 ,79 ,46 ,121 ,191 ,157 ,184 ,137 ,8 ,161 ,49 ,122 ,106 ,242 ,125 ,35 ,232 ,23 ,200 ,177 ,173 ,160 ,95 ,144 ,214 ,133 ,88 ,210 ,227 ,26 ,131 ,44 ,183 ,159 ,115 ,95 ,133 ,126 ,1 ,186 ,87 ,1 ,74 ,79 ,105 ,140 ,179 ,208 ,126 ,253 ,139 ,40 ,239 ,207 ,93 ,31 ,81 ,57 ,23 ,178 ,204 ,126 ,189 ,122 ,210 ,215 ,246 ,233 ,231 ,224 ,184 ,6 ,191 ,251 ,28 ,253 ,100 ,114 ,61 ,207 ,140 ,140 ,177 ,188 ,126 ,161 ,249 ,116 ,14 ,127 ,151 ,214 ,111 ,196 ,87 ,168 ,85 ,198 ,91 ,88 ,191 ,240 ,122 ,244 ,235 ,27 ,247 ,253 ,253 ,210 ,75 ,107 ,207 ,168 ,124 ,182 ,63 ,10 ,43 ,130 ,233 ,23 ,71 ,170 ,148 ,253 ,158 ,7 ,51 ,38 ,70 ,98 ,250 ,5 ,147 ,231 ,24 ,211 ,47 ,148 ,60 ,197 ,164 ,53 ,152 ,124 ,18 ,71 ,34 ,214 ,222 ,64 ,174 ,25 ,198 ,244 ,11 ,227 ,156 ,96 ,9 ,103 ,159 ,248 ,238 ,26 ,192 ,226 ,255 ,11 ,61 ,11 ,167 ,125 ,222 ,234 ,102 ,234 ,225 ,155 ,197 ,139 ,56 ,252 ,133 ,234 ,36 ,111 ,124 ,147 ,143 ,127 ,13 ,94 ,128 ,144 ,96 ,245 ,28 ,52 ,185 ,95 ,149 ,117 ,49 ,131 ,148 ,120 ,99 ,89 ,90 ,191 ,220 ,99 ,46 ,172 ,95 ,246 ,49 ,23 ,213 ,175 ,192 ,78 ,193 ,162 ,250 ,177 ,254 ,70 ,41 ,177 ,115 ,186 ,200 ,126 ,25 ,11 ,46 ,180 ,95 ,182 ,132 ,203 ,237 ,151 ,39 ,225 ,162 ,251 ,101 ,40 ,184 ,240 ,126 ,201 ,55 ,201 ,15 ,208 ,47 ,105 ,193 ,37 ,245 ,243 ,125 ,126 ,158 ,106 ,204 ,37 ,245 ,27 ,188 ,255 ,115 ,2 ,183 ,30 ,221 ,225 ,254 ,184 ,62 ,241 ,77 ,126 ,184 ,74 ,178 ,56 ,209 ,124 ,5 ,83 ,36 ,188 ,245 ,235 ,62 ,177 ,147 ,67 ,57 ,30 ,54 ,36 ,239 ,36 ,92 ,94 ,191 ,195 ,208 ,133 ,68 ,170 ,67 ,61 ,96 ,63 ,213 ,89 ,248 ,144 ,253 ,20 ,19 ,62 ,106 ,63 ,173 ,245 ,120 ,209 ,253 ,60 ,23 ,192 ,208 ,207 ,163 ,221 ,207 ,53 ,31 ,85 ,134 ,122 ,136 ,126 ,39 ,244 ,11 ,39 ,245 ,235 ,23 ,84 ,25 ,234 ,129 ,250 ,117 ,215 ,99 ,149 ,161 ,30 ,167 ,31 ,235 ,111 ,48 ,161 ,31 ,219 ,143 ,17 ,58 ,253 ,124 ,95 ,40 ,140 ,181 ,252 ,126 ,206 ,157 ,105 ,149 ,161 ,22 ,222 ,47 ,249 ,41 ,213 ,69 ,247 ,243 ,83 ,25 ,138 ,126 ,113 ,232 ,23 ,103 ,137 ,253 ,2 ,191 ,135 ,169 ,50 ,214 ,242 ,250 ,133 ,197 ,163 ,95 ,223 ,184 ,239 ,255 ,234 ,156 ,65 ,117 ,246 ,187 ,221 ,153 ,173 ,125 ,161 ,184 ,229 ,126 ,249 ,235 ,121 ,250 ,57 ,44 ,162 ,95 ,142 ,207 ,63 ,28 ,222 ,230 ,223 ,79 ,117 ,204 ,177 ,253 ,110 ,119 ,244 ,172 ,84 ,23 ,67 ,69 ,222 ,153 ,119 ,54 ,190 ,223 ,225 ,252 ,99 ,43 ,149 ,246 ,146 ,196 ,203 ,95 ,239 ,244 ,163 ,31 ,23 ,227 ,254 ,108 ,63 ,195 ,126 ,165 ,23 ,207 ,60 ,226 ,197 ,201 ,185 ,222 ,46 ,17 ,241 ,226 ,176 ,222 ,198 ,161 ,94 ,28 ,226 ,197 ,41 ,241 ,174 ,23 ,116 ,255 ,118 ,73 ,166 ,229 ,27 ,163 ,192 ,38 ,67 ,252 ,121 ,133 ,32 ,86 ,191 ,191 ,159 ,121 ,204 ,201 ,253 ,170 ,204 ,11 ,106 ,20 ,253 ,226 ,208 ,47 ,14 ,253 ,226 ,44 ,169 ,159 ,243 ,248 ,87 ,166 ,50 ,38 ,253 ,226 ,208 ,47 ,14 ,253 ,226 ,208 ,47 ,14 ,253 ,226 ,208 ,47 ,14 ,253 ,226 ,208 ,47 ,202 ,244 ,243 ,47 ,149 ,198 ,240 ,186 ,74 ,244 ,115 ,156 ,221 ,187 ,253 ,228 ,229 ,202 ,245 ,68 ,165 ,49 ,188 ,174 ,2 ,253 ,92 ,118 ,215 ,76 ,235 ,206 ,19 ,123 ,250 ,133 ,160 ,95 ,156 ,89 ,246 ,243 ,235 ,93 ,93 ,153 ,114 ,176 ,229 ,245 ,235 ,229 ,75 ,250 ,33 ,201 ,226 ,250 ,101 ,157 ,125 ,135 ,250 ,251 ,194 ,250 ,101 ,173 ,119 ,248 ,185 ,121 ,94 ,84 ,191 ,172 ,155 ,142 ,67 ,235 ,160 ,100 ,213 ,121 ,106 ,142 ,253 ,50 ,215 ,219 ,60 ,123 ,250 ,89 ,190 ,254 ,89 ,150 ,247 ,157 ,175 ,119 ,68 ,215 ,121 ,122 ,55 ,183 ,126 ,89 ,55 ,187 ,135 ,254 ,9 ,133 ,206 ,211 ,183 ,13 ,139 ,193 ,235 ,15 ,4 ,153 ,39 ,95 ,123 ,221 ,149 ,58 ,185 ,30 ,183 ,169 ,155 ,47 ,241 ,228 ,219 ,246 ,234 ,117 ,223 ,0 ,239 ,107 ,119 ,218 ,37 ,81 ,145 ,119 ,151 ,249 ,176 ,255 ,69 ,200 ,247 ,252 ,220 ,188 ,210 ,106 ,255 ,125 ,70 ,253 ,50 ,175 ,187 ,247 ,83 ,83 ,238 ,53 ,184 ,241 ,146 ,180 ,11 ,19 ,45 ,247 ,27 ,159 ,184 ,238 ,94 ,214 ,224 ,219 ,87 ,8 ,27 ,175 ,177 ,248 ,251 ,61 ,13 ,185 ,247 ,152 ,247 ,174 ,201 ,119 ,41 ,216 ,62 ,119 ,218 ,123 ,91 ,52 ,38 ,247 ,228 ,155 ,112 ,22 ,191 ,74 ,188 ,72 ,49 ,114 ,239 ,242 ,189 ,13 ,231 ,234 ,49 ,188 ,251 ,98 ,34 ,223 ,75 ,181 ,241 ,246 ,75 ,188 ,76 ,211 ,101 ,94 ,119 ,235 ,163 ,216 ,105 ,61 ,240 ,211 ,222 ,102 ,55 ,31 ,185 ,39 ,223 ,31 ,206 ,60 ,222 ,181 ,122 ,157 ,120 ,177 ,38 ,202 ,189 ,217 ,117 ,124 ,126 ,190 ,118 ,63 ,117 ,81 ,37 ,95 ,176 ,41 ,114 ,79 ,190 ,131 ,28 ,233 ,245 ,60 ,238 ,159 ,158 ,126 ,233 ,23 ,108 ,130 ,113 ,247 ,223 ,72 ,118 ,253 ,198 ,55 ,207 ,115 ,23 ,54 ,247 ,254 ,198 ,229 ,75 ,212 ,175 ,177 ,101 ,216 ,205 ,107 ,250 ,217 ,232 ,231 ,125 ,242 ,194 ,226 ,151 ,183 ,14 ,54 ,250 ,181 ,158 ,116 ,28 ,212 ,217 ,92 ,123 ,13 ,244 ,91 ,245 ,142 ,42 ,164 ,130 ,86 ,243 ,21 ,239 ,183 ,237 ,31 ,148 ,9 ,231 ,21 ,140 ,174 ,188 ,135 ,226 ,253 ,228 ,121 ,213 ,57 ,175 ,181 ,178 ,155 ,175 ,108 ,191 ,23 ,87 ,152 ,237 ,118 ,115 ,123 ,209 ,171 ,48 ,69 ,237 ,40 ,217 ,239 ,197 ,19 ,230 ,126 ,145 ,165 ,202 ,144 ,201 ,20 ,236 ,103 ,246 ,124 ,192 ,24 ,229 ,250 ,89 ,94 ,43 ,195 ,149 ,234 ,231 ,91 ,119 ,231 ,164 ,84 ,63 ,149 ,127 ,208 ,131 ,90 ,200 ,91 ,95 ,49 ,207 ,43 ,238 ,15 ,21 ,227 ,55 ,242 ,225 ,166 ,200 ,61 ,155 ,130 ,78 ,198 ,247 ,110 ,100 ,108 ,81 ,237 ,217 ,172 ,234 ,110 ,118 ,27 ,118 ,159 ,7 ,111 ,206 ,56 ,251 ,198 ,225 ,91 ,243 ,90 ,14 ,139 ,46 ,39 ,240 ,133 ,183 ,164 ,230 ,126 ,139 ,238 ,59 ,214 ,126 ,115 ,57 ,252 ,149 ,15 ,206 ,122 ,87 ,21 ,205 ,226 ,4 ,130 ,235 ,241 ,4 ,5 ,119 ,183 ,50 ,210 ,204 ,146 ,174 ,42 ,50 ,27 ,176 ,118 ,244 ,235 ,239 ,58 ,43 ,6 ,244 ,246 ,147 ,47 ,202 ,178 ,186 ,10 ,215 ,114 ,30 ,233 ,216 ,67 ,45 ,224 ,61 ,223 ,107 ,255 ,73 ,215 ,53 ,109 ,70 ,103 ,160 ,60 ,253 ,18 ,254 ,120 ,227 ,161 ,217 ,207 ,247 ,92 ,215 ,90 ,107 ,120 ,85 ,114 ,28 ,49 ,159 ,218 ,4 ,220 ,120 ,250 ,121 ,174 ,9 ,84 ,26 ,93 ,151 ,216 ,70 ,206 ,167 ,54 ,1 ,61 ,83 ,202 ,119 ,85 ,96 ,255 ,213 ,229 ,213 ,82 ,26 ,231 ,53 ,9 ,74 ,19 ,208 ,83 ,196 ,123 ,73 ,170 ,206 ,232 ,170 ,198 ,245 ,211 ,153 ,128 ,247 ,139 ,172 ,250 ,27 ,85 ,95 ,62 ,139 ,23 ,16 ,137 ,253 ,156 ,249 ,116 ,250 ,237 ,220 ,19 ,202 ,127 ,85 ,170 ,193 ,179 ,94 ,210 ,175 ,134 ,122 ,46 ,41 ,82 ,89 ,129 ,61 ,253 ,118 ,206 ,118 ,70 ,87 ,224 ,107 ,43 ,233 ,177 ,84 ,19 ,240 ,126 ,159 ,146 ,222 ,83 ,254 ,124 ,6 ,87 ,96 ,169 ,159 ,39 ,159 ,74 ,191 ,205 ,53 ,199 ,186 ,251 ,204 ,208 ,69 ,229 ,189 ,63 ,40 ,110 ,220 ,219 ,159 ,78 ,191 ,91 ,142 ,222 ,230 ,99 ,96 ,245 ,53 ,120 ,21 ,76 ,64 ,191 ,214 ,10 ,173 ,48 ,164 ,251 ,235 ,128 ,111 ,155 ,118 ,174 ,243 ,246 ,98 ,211 ,125 ,192 ,18 ,161 ,75 ,221 ,205 ,215 ,120 ,153 ,198 ,6 ,228 ,205 ,217 ,175 ,59 ,253 ,250 ,15 ,155 ,59 ,139 ,48 ,216 ,175 ,243 ,152 ,66 ,191 ,157 ,179 ,159 ,107 ,101 ,189 ,207 ,216 ,42 ,126 ,120 ,93 ,253 ,126 ,242 ,239 ,197 ,187 ,126 ,93 ,126 ,130 ,91 ,191 ,238 ,218 ,216 ,153 ,126 ,210 ,87 ,48 ,215 ,241 ,195 ,235 ,26 ,232 ,247 ,222 ,125 ,84 ,179 ,95 ,119 ,107 ,208 ,238 ,247 ,34 ,61 ,99 ,238 ,13 ,208 ,82 ,191 ,246 ,244 ,91 ,55 ,158 ,177 ,251 ,13 ,116 ,103 ,168 ,78 ,214 ,12 ,253 ,58 ,167 ,94 ,90 ,155 ,10 ,243 ,253 ,122 ,111 ,116 ,249 ,251 ,181 ,87 ,223 ,246 ,147 ,114 ,85 ,3 ,186 ,165 ,164 ,157 ,151 ,230 ,227 ,9 ,183 ,31 ,237 ,111 ,197 ,181 ,67 ,153 ,237 ,119 ,239 ,245 ,222 ,254 ,63 ,59 ,181 ,82 ,244 ,107 ,175 ,140 ,157 ,213 ,183 ,253 ,55 ,51 ,232 ,39 ,233 ,191 ,78 ,181 ,95 ,235 ,31 ,214 ,158 ,126 ,157 ,117 ,219 ,236 ,251 ,95 ,129 ,126 ,247 ,227 ,143 ,117 ,243 ,225 ,246 ,244 ,115 ,221 ,65 ,39 ,126 ,120 ,93 ,222 ,126 ,141 ,88 ,194 ,67 ,83 ,53 ,78 ,178 ,52 ,30 ,245 ,174 ,190 ,183 ,39 ,205 ,237 ,255 ,121 ,79 ,182 ,8 ,253 ,20 ,70 ,108 ,148 ,90 ,75 ,15 ,10 ,157 ,110 ,201 ,237 ,158 ,128 ,9 ,91 ,125 ,117 ,207 ,255 ,221 ,63 ,63 ,239 ,158 ,248 ,171 ,218 ,127 ,113 ,123 ,203 ,92 ,107 ,140 ,175 ,42 ,236 ,100 ,179 ,106 ,191 ,214 ,103 ,108 ,175 ,167 ,175 ,120 ,108 ,58 ,249 ,102 ,116 ,3 ,187 ,176 ,15 ,59 ,132 ,164 ,211 ,237 ,186 ,181 ,122 ,214 ,157 ,191 ,176 ,124 ,3 ,187 ,144 ,233 ,167 ,185 ,249 ,8 ,233 ,231 ,252 ,3 ,141 ,225 ,181 ,13 ,207 ,62 ,221 ,213 ,119 ,248 ,247 ,23 ,186 ,91 ,9 ,187 ,167 ,175 ,78 ,28 ,107 ,112 ,243 ,37 ,170 ,211 ,111 ,224 ,34 ,131 ,231 ,254 ,187 ,156 ,115 ,189 ,54 ,34 ,243 ,244 ,235 ,125 ,206 ,17 ,188 ,250 ,86 ,74 ,11 ,160 ,205 ,159 ,79 ,254 ,148 ,51 ,198 ,206 ,153 ,78 ,154 ,101 ,206 ,176 ,102 ,244 ,87 ,97 ,241 ,21 ,106 ,23 ,80 ,250 ,239 ,189 ,214 ,121 ,241 ,206 ,249 ,140 ,33 ,158 ,201 ,119 ,81 ,107 ,94 ,133 ,239 ,155 ,128 ,85 ,231 ,181 ,247 ,119 ,75 ,123 ,71 ,31 ,13 ,238 ,185 ,151 ,194 ,38 ,56 ,223 ,193 ,243 ,212 ,3 ,115 ,229 ,91 ,247 ,94 ,233 ,254 ,188 ,248 ,145 ,201 ,247 ,96 ,235 ,156 ,19 ,60 ,187 ,222 ,237 ,206 ,222 ,201 ,151 ,130 ,106 ,57 ,160 ,248 ,230 ,81 ,255 ,233 ,152 ,154 ,38 ,13 ,108 ,71 ,180 ,212 ,63 ,251 ,245 ,190 ,56 ,191 ,72 ,118 ,154 ,130 ,6 ,15 ,126 ,187 ,6 ,142 ,67 ,148 ,117 ,206 ,90 ,173 ,125 ,175 ,221 ,216 ,127 ,251 ,115 ,157 ,138 ,73 ,56 ,100 ,227 ,238 ,227 ,194 ,87 ,105 ,90 ,182 ,85 ,194 ,229 ,80 ,224 ,136 ,151 ,126 ,69 ,94 ,6 ,95 ,190 ,76 ,251 ,132 ,51 ,54 ,124 ,23 ,202 ,210 ,75 ,104 ,218 ,96 ,61 ,10 ,250 ,4 ,229 ,203 ,240 ,38 ,248 ,246 ,245 ,115 ,79 ,249 ,165 ,74 ,62 ,146 ,170 ,176 ,124 ,73 ,3 ,94 ,190 ,144 ,222 ,82 ,165 ,27 ,78 ,85 ,104 ,190 ,100 ,107 ,176 ,208 ,110 ,70 ,13 ,199 ,220 ,192 ,56 ,197 ,232 ,238 ,120 ,159 ,190 ,89 ,223 ,119 ,42 ,217 ,175 ,62 ,186 ,78 ,35 ,220 ,189 ,24 ,223 ,251 ,28 ,145 ,79 ,61 ,224 ,112 ,189 ,115 ,65 ,229 ,97 ,53 ,141 ,187 ,255 ,184 ,238 ,216 ,242 ,111 ,71 ,9 ,42 ,221 ,113 ,53 ,141 ,202 ,167 ,186 ,42 ,141 ,249 ,1 ,154 ,74 ,113 ,92 ,93 ,229 ,250 ,109 ,70 ,244 ,179 ,27 ,208 ,189 ,150 ,38 ,94 ,129 ,135 ,62 ,68 ,111 ,179 ,122 ,238 ,217 ,113 ,185 ,253 ,168 ,87 ,76 ,50 ,46 ,159 ,217 ,128 ,117 ,64 ,155 ,144 ,215 ,140 ,53 ,254 ,215 ,183 ,190 ,106 ,13 ,173 ,170 ,115 ,253 ,253 ,208 ,139 ,180 ,222 ,0 ,253 ,63 ,253 ,38 ,27 ,58 ,193 ,90 ,68 ,224 ,117 ,225 ,154 ,223 ,191 ,60 ,153 ,144 ,207 ,230 ,231 ,31 ,161 ,235 ,165 ,226 ,245 ,247 ,135 ,73 ,191 ,157 ,247 ,108 ,243 ,242 ,131 ,50 ,253 ,118 ,147 ,250 ,89 ,252 ,0 ,41 ,56 ,139 ,102 ,191 ,137 ,249 ,44 ,238 ,4 ,22 ,233 ,183 ,145 ,243 ,188 ,94 ,110 ,71 ,185 ,223 ,126 ,157 ,205 ,4 ,44 ,210 ,79 ,72 ,39 ,188 ,74 ,184 ,202 ,210 ,222 ,153 ,152 ,18 ,253 ,252 ,223 ,149 ,105 ,217 ,181 ,95 ,186 ,86 ,24 ,93 ,87 ,137 ,237 ,71 ,59 ,202 ,192 ,139 ,191 ,211 ,175 ,171 ,217 ,111 ,53 ,52 ,114 ,221 ,252 ,69 ,66 ,123 ,123 ,48 ,129 ,7 ,102 ,170 ,251 ,207 ,141 ,41 ,21 ,114 ,39 ,247 ,102 ,64 ,133 ,209 ,117 ,133 ,133 ,9 ,58 ,202 ,11 ,118 ,207 ,241 ,37 ,232 ,245 ,141 ,11 ,181 ,204 ,29 ,130 ,4 ,221 ,154 ,41 ,213 ,253 ,155 ,66 ,167 ,147 ,227 ,27 ,195 ,22 ,52 ,207 ,173 ,56 ,230 ,86 ,235 ,4 ,150 ,194 ,144 ,247 ,26 ,85 ,232 ,159 ,120 ,238 ,120 ,87 ,152 ,243 ,134 ,7 ,87 ,237 ,231 ,53 ,250 ,237 ,198 ,78 ,63 ,203 ,247 ,207 ,41 ,240 ,251 ,61 ,83 ,98 ,216 ,221 ,128 ,140 ,203 ,167 ,186 ,251 ,178 ,14 ,255 ,155 ,89 ,108 ,64 ,242 ,76 ,191 ,73 ,95 ,134 ,182 ,219 ,111 ,212 ,4 ,212 ,61 ,251 ,82 ,133 ,255 ,141 ,225 ,126 ,217 ,175 ,223 ,136 ,90 ,127 ,85 ,150 ,64 ,87 ,230 ,233 ,231 ,190 ,255 ,154 ,135 ,229 ,126 ,225 ,19 ,80 ,103 ,188 ,9 ,251 ,47 ,134 ,239 ,255 ,114 ,8 ,15 ,168 ,52 ,156 ,239 ,246 ,237 ,14 ,83 ,166 ,108 ,70 ,57 ,215 ,222 ,102 ,191 ,208 ,26 ,215 ,47 ,192 ,89 ,237 ,23 ,20 ,80 ,239 ,220 ,239 ,173 ,95 ,232 ,22 ,120 ,244 ,31 ,100 ,151 ,51 ,95 ,227 ,227 ,143 ,176 ,183 ,179 ,198 ,249 ,46 ,123 ,187 ,47 ,23 ,25 ,243 ,53 ,123 ,132 ,4 ,220 ,141 ,123 ,121 ,33 ,161 ,55 ,178 ,83 ,208 ,8 ,18 ,48 ,163 ,54 ,141 ,23 ,175 ,85 ,151 ,67 ,89 ,158 ,201 ,119 ,232 ,126 ,237 ,210 ,63 ,167 ,218 ,175 ,93 ,235 ,46 ,136 ,182 ,180 ,191 ,186 ,117 ,247 ,220 ,241 ,34 ,254 ,206 ,229 ,118 ,219 ,187 ,190 ,87 ,127 ,81 ,180 ,117 ,19 ,38 ,25 ,164 ,155 ,229 ,148 ,240 ,243 ,62 ,108 ,77 ,210 ,197 ,209 ,73 ,150 ,38 ,133 ,58 ,233 ,151 ,6 ,118 ,66 ,154 ,16 ,70 ,247 ,254 ,178 ,155 ,114 ,245 ,223 ,172 ,166 ,95 ,106 ,203 ,185 ,126 ,173 ,144 ,197 ,92 ,63 ,89 ,202 ,132 ,124 ,54 ,119 ,158 ,135 ,14 ,60 ,18 ,109 ,143 ,135 ,126 ,41 ,74 ,160 ,53 ,180 ,174 ,66 ,253 ,198 ,125 ,123 ,198 ,110 ,190 ,98 ,253 ,198 ,78 ,64 ,171 ,111 ,126 ,165 ,250 ,181 ,46 ,11 ,26 ,246 ,77 ,111 ,96 ,93 ,197 ,250 ,141 ,10 ,104 ,119 ,215 ,165 ,92 ,191 ,198 ,105 ,229 ,33 ,95 ,236 ,93 ,184 ,123 ,85 ,176 ,95 ,240 ,110 ,160 ,238 ,160 ,186 ,138 ,246 ,11 ,10 ,184 ,86 ,30 ,83 ,87 ,217 ,126 ,115 ,252 ,201 ,208 ,182 ,210 ,253 ,78 ,95 ,247 ,216 ,202 ,211 ,112 ,37 ,158 ,22 ,52 ,166 ,124 ,191 ,147 ,211 ,9 ,191 ,86 ,187 ,211 ,3 ,137 ,198 ,210 ,101 ,163 ,223 ,167 ,198 ,201 ,83 ,251 ,243 ,238 ,202 ,80 ,191 ,5 ,234 ,125 ,34 ,82 ,122 ,129 ,230 ,133 ,233 ,23 ,165 ,91 ,207 ,238 ,81 ,128 ,69 ,76 ,190 ,24 ,189 ,119 ,62 ,38 ,223 ,24 ,108 ,56 ,162 ,36 ,157 ,124 ,211 ,62 ,125 ,59 ,209 ,92 ,138 ,132 ,82 ,79 ,190 ,201 ,253 ,42 ,229 ,5 ,73 ,35 ,249 ,59 ,223 ,162 ,251 ,101 ,120 ,231 ,91 ,114 ,191 ,28 ,27 ,142 ,5 ,247 ,235 ,230 ,75 ,178 ,215 ,178 ,216 ,126 ,153 ,246 ,90 ,150 ,217 ,47 ,223 ,46 ,223 ,34 ,251 ,37 ,223 ,234 ,222 ,45 ,177 ,95 ,198 ,124 ,11 ,236 ,215 ,173 ,151 ,246 ,112 ,109 ,113 ,253 ,114 ,78 ,190 ,195 ,226 ,250 ,101 ,217 ,103 ,105 ,152 ,126 ,252 ,91 ,37 ,94 ,178 ,73 ,50 ,79 ,190 ,67 ,235 ,131 ,34 ,151 ,95 ,63 ,150 ,171 ,255 ,169 ,166 ,197 ,143 ,149 ,122 ,111 ,125 ,94 ,25 ,151 ,107 ,211 ,203 ,103 ,241 ,10 ,212 ,113 ,249 ,242 ,245 ,19 ,174 ,75 ,48 ,121 ,21 ,145 ,209 ,126 ,194 ,183 ,103 ,108 ,254 ,18 ,136 ,205 ,126 ,253 ,122 ,86 ,175 ,0 ,180 ,216 ,79 ,216 ,64 ,175 ,179 ,12 ,60 ,129 ,189 ,126 ,251 ,109 ,63 ,95 ,149 ,97 ,220 ,105 ,204 ,245 ,19 ,46 ,198 ,178 ,124 ,37 ,145 ,181 ,126 ,253 ,201 ,183 ,178 ,156 ,207 ,88 ,63 ,97 ,221 ,93 ,89 ,220 ,105 ,190 ,51 ,213 ,175 ,255 ,197 ,204 ,23 ,235 ,151 ,178 ,25 ,234 ,183 ,19 ,246 ,90 ,140 ,215 ,179 ,212 ,79 ,56 ,226 ,168 ,18 ,14 ,167 ,196 ,76 ,63 ,97 ,246 ,153 ,159 ,124 ,118 ,8 ,103 ,11 ,214 ,165 ,151 ,105 ,62 ,234 ,159 ,253 ,201 ,103 ,245 ,128 ,205 ,34 ,225 ,251 ,11 ,165 ,23 ,105 ,78 ,250 ,235 ,174 ,233 ,159 ,188 ,180 ,70 ,88 ,119 ,77 ,158 ,170 ,82 ,144 ,96 ,90 ,204 ,115 ,183 ,101 ,34 ,253 ,126 ,194 ,91 ,223 ,18 ,119 ,91 ,234 ,52 ,251 ,127 ,219 ,254 ,91 ,223 ,18 ,235 ,221 ,40 ,247 ,155 ,221 ,217 ,130 ,88 ,170 ,253 ,102 ,118 ,166 ,207 ,154 ,185 ,157 ,233 ,139 ,162 ,254 ,254 ,39 ,220 ,67 ,204 ,250 ,169 ,170 ,104 ,138 ,215 ,38 ,236 ,122 ,245 ,94 ,150 ,59 ,249 ,62 ,213 ,154 ,27 ,223 ,126 ,190 ,231 ,20 ,251 ,150 ,118 ,212 ,154 ,71 ,4 ,66 ,189 ,48 ,149 ,226 ,66 ,100 ,166 ,121 ,71 ,202 ,169 ,249 ,230 ,220 ,239 ,112 ,122 ,251 ,83 ,250 ,7 ,61 ,96 ,63 ,213 ,237 ,239 ,3 ,246 ,83 ,69 ,191 ,56 ,244 ,139 ,67 ,191 ,56 ,244 ,139 ,67 ,191 ,56 ,244 ,139 ,67 ,191 ,40 ,11 ,251 ,254 ,71 ,118 ,244 ,139 ,20 ,240 ,253 ,25 ,89 ,233 ,5 ,7 ,0 ,96 ,246 ,222 ,74 ,47 ,192 ,204 ,209 ,47 ,14 ,253 ,38 ,218 ,95 ,126 ,224 ,173 ,244 ,114 ,204 ,28 ,243 ,47 ,206 ,190 ,244 ,2 ,204 ,213 ,158 ,163 ,217 ,40 ,251 ,243 ,221 ,238 ,249 ,130 ,194 ,84 ,199 ,247 ,121 ,221 ,227 ,222 ,152 ,253 ,243 ,106 ,207 ,202 ,27 ,97 ,251 ,252 ,242 ,235 ,82 ,191 ,161 ,144 ,195 ,251 ,105 ,14 ,86 ,165 ,151 ,98 ,182 ,46 ,231 ,239 ,45 ,222 ,77 ,8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,56 ,0 ,192 ,0 ,56 ,0 ,192 ,0 ,0 ,80 ,192 ,0 ,0 ,160 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }; 

#endif
unsigned int wrSplashDeflatedSize = sizeof(wrSplashDeflatedData);

#endif /* __INC_gfxSplash_h */
