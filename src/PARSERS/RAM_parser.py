import csv
import sys


def column_parsing_psscan(file_to_read, file_to_write):
    with open(file_to_read, "r") as file:
        reader = csv.reader(file)

        with open(file_to_write, "w") as target_file:
            count = 0
        
            writer = csv.writer(target_file)
            
            for row in reader:
                splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))
            
                if count == 1:
                    count += 1
                    continue
                elif  count == 0:
                    splitedRow[5] = splitedRow[5] + " " + splitedRow[6]
                    splitedRow[6] = splitedRow[7] + " " + splitedRow[8]
                    splitedRow = splitedRow[:-2]
                else:
                    if len(splitedRow) >= 8:
                        splitedRow[5] = splitedRow[5] + " " + splitedRow[6] + " " + splitedRow[7]
                        splitedRow.pop(6)
                        splitedRow.pop(6)
                        if len(splitedRow) > 6:
                            splitedRow[6] = splitedRow[6] + " " + splitedRow[7] + " " + splitedRow[8]
                            splitedRow = splitedRow[:-2]
                        
                writer.writerow(splitedRow)
                count += 1

def column_parsing_pslist(file_to_read, file_to_write):
    with open(file_to_read, "r") as file:
        reader = csv.reader(file)

        with open(file_to_write, "w") as target_file:
            count = 0
        
            writer = csv.writer(target_file)
            
            for row in reader:
                splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))
            
                if count == 1:
                    count += 1
                    continue
                    
                if (len(splitedRow) >= 11):
                    splitedRow[8] = splitedRow[8] + " " + splitedRow[9] +  " "  + splitedRow[10]
                    splitedRow.pop(9)
                    splitedRow.pop(9)

                if (len(splitedRow) == 12):
                    splitedRow[9] = splitedRow[9] + " " + splitedRow[10] + " " + splitedRow[11]
                    splitedRow.pop(10)
                    splitedRow.pop(10)
                    
                writer.writerow(splitedRow)
                count += 1
                print(splitedRow)
            


def column_parsing_sockscan(file_to_read, file_to_write):
    with open(file_to_read, "r") as file:
        reader = csv.reader(file)

        with open(file_to_write, "w") as target_file:
            count = 0
        
            writer = csv.writer(target_file)
            
            for row in reader:
                splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))
            
                if count == 1:
                    count += 1
                    continue
                elif count == 0:
                    splitedRow[-2] = splitedRow[-2] + " " + splitedRow[-1]
                    splitedRow.pop(-1)
                else:
                    splitedRow[-3] = splitedRow[-3] + " " + splitedRow[-2] + " " + splitedRow[-1]
                    splitedRow.pop(-1)
                    splitedRow.pop(-1)
                    
                writer.writerow(splitedRow)
                count += 1
                print(splitedRow)
            

if __name__ == "__main__": 
    if (len(sys.argv) == 3):
        column_parsing_psscan(sys.argv[1], sys.argv[2])
    elif (len(sys.argv) == 4):
        if (sys.argv[3] == "psscan"):
            column_parsing_psscan(sys.argv[1], sys.argv[2])
        elif (sys.argv[3] == "pslist"):
            column_parsing_pslist(sys.argv[1], sys.argv[2])
        else:
            column_parsing_sockscan(sys.argv[1], sys.argv[2])
    else:
        print("Argument error!")
