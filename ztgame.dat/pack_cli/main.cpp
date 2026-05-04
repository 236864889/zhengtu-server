#include "../engine/include/engine.h"
#include "../engine/include/PackAggregate.h"
#include <iostream>

static void usage()
{
    std::cout << "pack_cli --input <dir> --output <pak> [--password <pwd>] [--zip 1|0] [--encrypt 0|1|2] [--level 0-9]" << std::endl;
}

static bool onMakeSubFile(const char* fileName,DWORD dwCRC)
{
    std::cout << "[pack] file=" << fileName << " crc=" << dwCRC << std::endl;
    return true;
}

int main(int argc,char** argv)
{
    String input;
    String output;
    String password = PackAggregate::GetDefaultPassword();
    int zip = 1;
    int encrypt = 1;
    int level = 9;

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"--input") == 0 && i+1 < argc) input = argv[++i];
        else if(strcmp(argv[i],"--output") == 0 && i+1 < argc) output = argv[++i];
        else if(strcmp(argv[i],"--password") == 0 && i+1 < argc) password = argv[++i];
        else if(strcmp(argv[i],"--zip") == 0 && i+1 < argc) zip = atoi(argv[++i]);
        else if(strcmp(argv[i],"--encrypt") == 0 && i+1 < argc) encrypt = atoi(argv[++i]);
        else if(strcmp(argv[i],"--level") == 0 && i+1 < argc) level = atoi(argv[++i]);
        else { usage(); return 1; }
    }

    if(input.empty() || output.empty())
    {
        usage();
        return 1;
    }

    BOOL bStop = FALSE;
    std::vector<String> filters;
    bool ok = PackAggregate::MakePackDir(input.c_str(),output.c_str(),zip != 0,encrypt,level,password.c_str(),filters,NULL,bStop,onMakeSubFile);

    std::cout << "[pack] input=" << input << " output=" << output
              << " password_len=" << password.length() << " zip=" << zip
              << " encrypt=" << encrypt << " level=" << level << std::endl;
    std::cout << "[pack] result=" << (ok ? "success" : "failed") << std::endl;
    return ok ? 0 : 2;
}
