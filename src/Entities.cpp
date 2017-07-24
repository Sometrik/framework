#include <Entities.h>

#include <utf8.h>
#include <cassert>

using namespace std;

Entities * Entities::instance = 0;

static inline int get_xdigit(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else {
    return 0;
  }
}

Entities::Entities() {
  entities["rsquor"] = 0x2019;
  entities["rsquor"] = 0x201a;
  entities["rdquor"] = 0x201d;
  entities["ldquor"] = 0x201e;

  entities["quot"] = entities["quo"] = 34;
  entities["amp"] = entities["AMP"] = 38;
  entities["apos"] = 39;
  entities["lt"] = 60;
  entities["gt"] = entities["GT"] = 62;
  entities["nbsp"] = 160;
  entities["NBSP"] = 160; // non-standard
  entities["iexcl"] = 161;
  entities["cent"] = 162;
  entities["pound"] = 163;
  entities["curren"] = 164;
  entities["yen"] = 165;
  entities["brvbar"] = 166;
  entities["sect"] = 167;
  entities["uml"] = 168;
  entities["copy"] = 169;
  entities["ordf"] = 170;
  entities["laquo"] = 171;
  entities["not"] = 172;
  entities["shy"] = 173;
  entities["reg"] = 174;
  entities["macr"] = 175;
  entities["deg"] = 176;
  entities["plusmn"] = 177;
  entities["sup2"] = 178;    
  entities["sup3"] = 179;
  entities["acute"] = 180;
  entities["micro"] = 181;
  entities["para"] = 182;
  entities["middot"] = 183;
  entities["cedil"] = 184;
  entities["sup1"] = 185;
  entities["ordm"] = 186;
  entities["raquo"] = 187;
  entities["frac14"] = 188;
  entities["frac12"] = 189;
  entities["frac34"] = 190;
  entities["iquest"] = 191;
  entities["Agrave"] = 192;
  entities["Aacute"] = 193;
  entities["Acirc"] = 194;
  entities["Atilde"] = 195;
  entities["Auml"] = 196;
  entities["Aring"] = 197;
  entities["AElig"] = 198;
  entities["Ccedil"] = 199;
  entities["Egrave"] = 200;
  entities["Eacute"] = 201;
  entities["Ecirc"] = 202;
  entities["Euml"] = 203;
  entities["Igrave"] = 204;
  entities["Iacute"] = 205;
  entities["Icirc"] = 206;
  entities["Iuml"] = 207;
  entities["ETH"] = 208;
  entities["Ntilde"] = 209;
  entities["Ograve"] = 210;
  entities["Oacute"] = 211;
  entities["Ocirc"] = 212;
  entities["Otilde"] = 213;
  entities["Ouml"] = 214;
  entities["times"] = 215;
  entities["Oslash"] = 216;
  entities["Ugrave"] = 217;
  entities["Uacute"] = 218;
  entities["Ucirc"] = 219;
  entities["Uuml"] = 220;   
  entities["Yacute"] = 221;
  entities["THORN"] = 222;
  entities["szlig"] = 223;
  entities["agrave"] = 224;
  entities["aacute"] = 225;
  entities["acirc"] = 226;
  entities["atilde"] = 227;
  entities["auml"] = 228;
  entities["aring"] = 229;
  entities["aelig"] = 230;
  entities["ccedil"] = 231;
  entities["egrave"] = 232;
  entities["eacute"] = 233;
  entities["ecirc"] = 234;
  entities["euml"] = 235;
  entities["igrave"] = 236;
  entities["iacute"] = 237;
  entities["icirc"] = 238;
  entities["iuml"] = 239;
  entities["eth"] = 240;
  entities["ntilde"] = 241;
  entities["ograve"] = 242;
  entities["oacute"] = 243;
  entities["ocirc"] = 244;
  entities["otilde"] = 245;
  entities["ouml"] = 246;
  entities["divide"] = 247;
  entities["oslash"] = 248;
  entities["ugrave"] = 249;
  entities["uacute"] = 250;
  entities["ucirc"] = 251;
  entities["uuml"] = 252;
  entities["yacute"] = 253;
  entities["thorn"] = 254;
  entities["yuml"] = 255;
  entities["OElig"] = 338;
  entities["oelig"] = 339;
  entities["Scaron"] = 352;
  entities["scaron"] = 353;
  entities["Yuml"] = 376;
  entities["fnof"] = 402;
  entities["Alpha"] = 913;
  entities["Beta"] = 914;
  entities["Gamma"] = 915;
  entities["Delta"] = 916;
  entities["Epsilon"] = 917;
  entities["Zeta"] = 918;
  entities["Eta"] = 919;
  entities["Theta"] = 920;
  entities["Iota"] = 921;
  entities["Kappa"] = 922;
  entities["Lambda"] = 923;
  entities["Mu"] = 924;
  entities["Nu"] = 925;
  entities["Xi"] = 926;
  entities["Omicron"] = 927;
  entities["Pi"] = 928;
  entities["Rho"] = 929;
  // no Sigmaf
  entities["Sigma"] = 931;
  entities["Tau"] = 932;
  entities["Upsilon"] = 933;
  entities["Phi"] = 934;
  entities["Chi"] = 935;
  entities["Psi"] = 936;
  entities["Omega"] = 937;
  entities["alpha"] = 945;
  entities["beta"] = 946;
  entities["gamma"] = 947;
  entities["delta"] = 948;
  entities["epsilon"] = 949;
  entities["zeta"] = 950;
  entities["eta"] = 951;
  entities["theta"] = 952;
  entities["iota"] = 953;
  entities["kappa"] = 954;
  entities["lambda"] = 955;
  entities["mu"] = 956;
  entities["nu"] = 957;  
  entities["xi"] = 958;
  entities["omicron"] = 959;
  entities["pi"] = 960;
  entities["rho"] = 961;
  entities["sigmaf"] = 962;
  entities["sigma"] = 963;
  entities["tau"] = 964;
  entities["upsilon"] = 965;
  entities["phi"] = 966;
  entities["chi"] = 967;
  entities["psi"] = 968;
  entities["omega"] = 969;
  entities["ensp"] = 8194; // em space
  entities["emsp"] = 8195; // em space
  entities["thinsp"] = 8201;
  entities["lrm"] = 8206; // LEFT-TO-RIGHT MARK
  entities["rlm"] = 8207; // RIGHT-TO-LEFT MARK
  entities["dash"] = 8208;
  entities["ndash"] = 8211;
  entities["mdash"] = 8212;
  entities["lsquo"] = 8216;
  entities["rsquo"] = 8217;
  entities["sbquo"] = 8218;
  entities["ldquo"] = 8220;
  entities["rdquo"] = 8221;
  entities["bdquo"] = 8222;
  entities["dagger"] = 8224;
  entities["Dagger"] = 8225;
  entities["bull"] = entities["bullet"] = 8226; // bullet is not valid
  entities["hellip"] = entities["helip"] = 8230; // helip is typo
  entities["prime"] = 8242;
  entities["Prime"] = 8243;
  entities["lsaquo"] = 8249;
  entities["rsaquo"] = 8250;
  entities["oline"] = 8254;
  entities["frasl"] = 8260;
  entities["euro"] = 8364;
  entities["image"] = 8465;
  entities["weierp"] = 8472;
  entities["real"] = 8476;
  entities["trade"] = 8482;
  entities["alefsym"] = 8501;
  entities["larr"] = 8592;
  entities["uarr"] = 8593;
  entities["rarr"] = 8594;
  entities["darr"] = 8595;
  entities["harr"] = 8596;
  entities["map"] = 8614;
  entities["crarr"] = 8629;
  entities["lArr"] = 8656;
  entities["uArr"] = 8657;
  entities["rArr"] = 8658;
  entities["dArr"] = 8659;
  entities["hArr"] = 8660;
  entities["forall"] = 8704;
  entities["part"] = 8706;
  entities["exist"] = 8707;
  entities["empty"] = 8709;
  entities["nabla"] = 8711;
  entities["isin"] = 8712;
  entities["notin"] = 8713;
  entities["minus"] = 8722;
  entities["radic"] = 8730;
  entities["infin"] = 8734;
  entities["or"] = 8744;
  entities["cap"] = 8745;
  entities["sim"] = 8764;
  entities["cong"] = 8773;
  entities["asymp"] = 8776;
  entities["ne"] = 8800;
  entities["equiv"] = 8801;
  entities["le"] = 8804;
  entities["ge"] = 8805;
  entities["sub"] = 8834;
  entities["sup"] = 8835;
  entities["nsub"] = 8836;
  entities["supe"] = 8839;
  entities["oplus"] = 8853;
  entities["otimes"] = 8855;
  entities["perp"] = 8869;
  entities["sdot"] = 8901;
  entities["loz"] = 9674;
  entities["spades"] = 9824;
  entities["clubs"] = 9827;
  entities["hearts"] = 9829;
  entities["diams"] = 9830;
  // entities["actionquot"] = 
}

int
Entities::convertEntity(const std::string & entity) {
  int value = 0;
  if (!entity.empty()) {
    if (entity[0] == '#') {
      if (!entity.empty() && (entity[1] == 'x' || entity[1] == 'X')) {
	for (int i = 2; i < entity.size(); i++) {
	  value = 16 * value + get_xdigit(entity[i]);
	}
      } else {
	string part = entity.substr(1);
	value = atoi(part.c_str());
      }
    } else {
      auto it = entities.find(entity);
      if (it != entities.end()) {
	value = it->second;
      }
    }
  }

  return value;
}
