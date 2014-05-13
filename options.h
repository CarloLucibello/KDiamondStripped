#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include "options_parser.h"

namespace options{

enum  optionIndex { UNKNOWN, SEEDP, SEEDG, WORK, QI, HELP, VERBOSE, OUTPATH};
const option::Descriptor usage[] = {
{ UNKNOWN, 0,"", "", MyArg::Unknown, "USAGE: myprog [options]\n\n"
                                          "Options:" },
{ SEEDP, 0,"p","seedp", MyArg::Numeric, "  -p <num>, \t--seedp=<num>  "
											"\t Seed del player." },
{ SEEDG, 0,"g","seedg", MyArg::Numeric, "  -g <num>, \t--seedg=<num>  "
                                            "\t Seed del gioco." },
{ WORK, 0,"w","work", MyArg::NonEmpty, "  -w <string>, \t--work=<string>  "
                                    "\tLavoro da fare [play, test]." },
{ VERBOSE,    0,"v", "verbose", MyArg::None, "   -v ,\t --verbose "
                                        "\t Se NON dai la flag stampa info di debug su shell." },
{ QI, 0,"q","qi", MyArg::Numeric, "  -q <num>, \t--qi=<num>  "
                                            "\t Intelligenza del giocatore random. Deve essere compresa in [0,1]." },
{ OUTPATH, 0,"o","out", MyArg::NonEmpty, "  -o <path>, \t--out=<path>  "
											"\tOutput path." },

{ HELP,    0,"h", "help", MyArg::None,    " -h , \t--help  \tPrint usage and exit." },

{ UNKNOWN, 0,"", "", MyArg::None,
 "\nExamples:\n"
},
{ 0, 0, 0, 0, 0, 0 } };

class Opt {
public:
	//options
    int& seedPlayer;
    int& seedGame;
	string& work;     // "play", "test"
	bool& verbose;
	double& qi;
	string& outPath;


	int parseOptions(int argc, char* argv[]){
		int mand = 0;	//MODIFICA QUI : number of mandatory arguments
		argc -= mand + 1; argv += mand + 1;
//		argc-= mc; argv += mv; // skip program name argv[0] if present
	  option::Stats stats(usage, argc, argv);
	  option::Option options[stats.options_max], buffer[stats.buffer_max];
	  option::Parser parse(usage, argc, argv, options, buffer);

	  if (parse.error())
	    exit(1);

//	  if (options[HELP])
	  if (options[HELP] || argc < 0) // if any mandatory argument
	  {
	    int columns = getenv("COLUMNS")? atoi(getenv("COLUMNS")) : 80;
	    option::printUsage(fwrite, stdout, usage, columns);
	    exit(1);
	  }

	  for (int i = 0; i < parse.optionsCount(); ++i){
	    option::Option& opt = buffer[i];
	    switch (opt.index()){
	      case SEEDP:
	    	  seedPlayer = atoi(opt.arg);
	    	  break;

	      case SEEDG:
	    	  seedGame = atoi(opt.arg);
	    	  break;

          case WORK:
            work = opt.arg;
            break;

          case VERBOSE:
            verbose = false;
            break;

            case OUTPATH:
                outPath = opt.arg;
                break;

            case QI:
                qi = atof(opt.arg);
                break;
	    }
	  }


	  for (int i = 0; i < parse.nonOptionsCount(); ++i){
		  fprintf(stdout, "# Non-option argument #%d is %s\n", i, parse.nonOption(i));
	  }

	  return 0;
	}
};



} //options namespace


#endif // OPTIONS_MINCUT_H_INCLUDED
