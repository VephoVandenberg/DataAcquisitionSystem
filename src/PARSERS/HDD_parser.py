import os
import sys


def print_msg(err_string):
    if len(sys.argv) == 4:
        print(err_string)

def make_unicode(string):
    if not isinstance(string, unicode):
        string = string.decode("utf-8")
    return string

def parse_file(file_name_arg):
    f_name = make_unicode(file_name_arg)
    try:
        parser = createParser(f_name)
    except InputStreamError, err:
        print_msg("stream error" + unicode(err)  + "\n")
        return ["No_file"]
    return extract_metadata(parser)

def extract_metadata(parser):
    if not parser:
        return ["None"]
    else:
        try:
            metadata = extractMetadata(parser)
        except HachoirError, err:
            if len(sys.argv) == 4:
                print_msg("stream error" + unicode(err))
            return ["None"]

        if metadata is None:
            pritn(u"Unable to extract metadata")
            return ["None"]

        return str(metadata).split('\n- ')[1:]

def main_parse(flist, fname):
    print(u"HDD initialization mode")


if __name__ == "__main__":
    if len(sys.argv) == 3 or len(sys.argv) == 4:
        main_parse(sys.argv[1], sys.argv[2])
    else:
        print(u"Argument error!")
    
    
