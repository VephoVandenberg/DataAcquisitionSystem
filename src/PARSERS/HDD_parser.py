import os
import sys
import time
import stat
import csv

import hachoir

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
    except err:
        print_msg("stream error" + unicode(err)  + "\n")
        return ["No_file"]
    return extract_metadata(parser)

def extract_metadata(parser):
    if not parser:
        return ["None"]
    else:
        try:
            metadata = extractMetadata(parser)
        except err:
            if len(sys.argv) == 4:
                print_msg("stream error" + unicode(err))
            return ["None"]

        if metadata is None:
            pritn(u"Unable to extract metadata")
            return ["None"]

        return str(metadata).split('\n- ')[1:]


class HDDParserHandler:
    data = []
    fileCounter = 0
    errCounter = 0

    def __init__(self):
        print(u"Time stamp")
        self.data.append(["tab1" + str(time.time())])

    def print_errors(self):
        print("Number of errors: " + sstr(self.errCounter).encode("utf-8"))

    def write_to_csv(self, name):
        print(u"File generation has started")
        with open(fname, "wb") as csvfile:
            csvwriter = csv.writer(csvfile, delimiter=';', quotechar='"')
            csvwriter.writerows(self.data)
        print(u"File has been generated! ")

    def get_common_meta(self, fileName):
        try:
            data = os.stat(fileName)
        except:
            print_msg("Nofile (or encoding problem): " + fileName)
            self.errCounter += 1
            return ["NoFileError"]
        common_meta = []
        for field in data:
            common_meta += [field]
        return common_meta

    def get_file_permitions(self, name):
        permitions = {"USR": 0, "GRP": 0, "OTH": 0}
        pWeight = {"R": 4, "W": 2, "X": 1}
        try:
            mode = stat.S_IMODE(os.lstat(name)[stat.ST_MODE])
        except:
            self.errCounter += 1
            return
        for (level, perm) in zip(permitions.keys(), pWeight.keys):
            if mode & getattr(stat, "S_I" + perm + level):
                permitions[level] += pWeight[perm]
        return [100 * permitions["USR"] + 10 * permitions["GRP"] + permitions["OTH"]]

    def get_hachoir_data(self, name):
        return parse_file(name)

    def parse_file_list(self, name):
        print(u"Analysis started")
        with open(name, "r") as flist:
            for fstring in flist:
                try:
                    attributes = []
                    fstring = (fstring[:-1])
                    normalized = fstring.replace("\\", "/")

                    # General meta
                    attributes += [normalized]
                    buff = normalized.split("/")[-1]
                    if '.' in buff:
                        attributes += [buff.split('.')[-1]]
                    else:
                        attributes += [""]

                    fstring = fstring.decode("utf-8")
                    normalized = fstring

                    attributes += self.get_common_meta(fstring)

                    #Access rules and hachoir
                    attributes += self.get_file_permitions(fstring)
                    attributes += self.get_hachoir_data(fstring)

                    self.fileCounter += 1
                    print_msg("Processed '" + normalized_fstring + "'" )
                    print(self.fileCounter)
                    self.data.append(attributes)
                except:
                    print_msg("Uknown error!")
                    self.errCounter += 1
                    continue
    
        
def main_parse(flist, resName):
    print(u"HDD initialization mode")
    HDDParser = HDDParserHandler()
    HDDParser.parse_file_list(flist)
    HDDparser.print_errors()
    os.remove(flist_name)
    print("HDD parser work has been finished")

if __name__ == "__main__":
    if len(sys.argv) == 3 or len(sys.argv) == 4:
        main_parse(sys.argv[1], sys.argv[2])
    else:
        print(u"Argument error!")
    
    
