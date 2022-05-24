import csv
import sys


def column_parsing_all(file_to_read, file_to_write1, file_to_write2, mode):
    with open(file_to_read, "r") as file:
        reader = csv.reader(file)

        if (mode == "all" or "u" in mode or "t" in mode):
            
        # TCP/UDP  connectinos
            with open(file_to_write1, "w") as target_file:
                count = 0
            
                writer = csv.writer(target_file)

                for row in reader:
                    splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))

                    if "Active Internet connections" in row[0]:
                        continue
                    elif "Active UNIX domain sockets" in row[0]:
                        break;
                
                    if (count == 0):
                    
                        splitedRow[3] = splitedRow[3] + " " + splitedRow[4]
                        splitedRow.pop(4)
                        splitedRow[4] = splitedRow[4] + " " + splitedRow[5]
                        splitedRow.pop(5)
                        count += 1
                    
                
                    writer.writerow(splitedRow)
                    print(splitedRow)
                        
        if (mode == "all" or "x" in mode):
            #UNIX connections
            with open(file_to_write2, "w") as target_file:
                count = 0
            
                writer = csv.writer(target_file)

                for row in reader:
                    splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))

                    if count == 0:
                        count += 1
                        print(splitedRow)
                        writer.writerow(splitedRow)
                        continue
                
                    if "/" not in splitedRow[-1]:
                        splitedRow.append("")

                    
                    if splitedRow[3] != "]":
                        splitedRow[2] = splitedRow[2] + splitedRow[3] + splitedRow[4]
                        splitedRow.pop(3)
                        splitedRow.pop(3)
                    else:
                        splitedRow[2] = splitedRow[2] + splitedRow[3]
                        splitedRow.pop(3)


                    if ('CONNECTED' not in splitedRow) and ( 'LISTENING'  not in splitedRow):
                        splitedRow.insert(-4, "")

                    if "DGRAM" in splitedRow:
                        splitedRow[2], splitedRow[3] = splitedRow[3], splitedRow[2]

                    writer.writerow(splitedRow)
                    print(splitedRow)


                
        
def column_parsing_listen(file_to_read, file_to_write1, file_to_write2, mode):
    with open(file_to_read, "r") as file:            
        reader = csv.reader(file)

        if mode == "listen" or ( "u" in mode or "t" in mode):
            # TCP/UDP  connectinos
            with open(file_to_write1, "w") as target_file:
                count = 0
            
                writer = csv.writer(target_file)

                for row in reader:
                    splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))

                    if "Active Internet connections" in row[0]:
                        continue
                    elif "Active UNIX domain sockets" in row[0]:
                        break;
                
                    if (count == 0):
                    
                        splitedRow[3] = splitedRow[3] + " " + splitedRow[4]
                        splitedRow.pop(4)
                        splitedRow[4] = splitedRow[4] + " " + splitedRow[5]
                        splitedRow.pop(5)
                        count += 1

                    if splitedRow[-1] != "LISTEN":
                        splitedRow.append("")
                
                    writer.writerow(splitedRow)
                    print(splitedRow)
                        
        if mode == "listen" or "x" in mode:
            #UNIX connections
            
            with open(file_to_write2, "w") as target_file:
                count = 0
            
                writer = csv.writer(target_file)

                for row in reader:
                    splitedRow = list(filter(lambda x: x != "", row[0].split(" ")))

                    if "Active" in row[0]:
                        continue
                    
                    if count == 0:
                        count += 1
                        print(splitedRow)
                        writer.writerow(splitedRow)
                        continue

                    
                    if "/" not in splitedRow[-1]:
                        splitedRow.append("")

                        
                    
                    if splitedRow[3] != "]":
                        splitedRow[2] = splitedRow[2] + splitedRow[3] + splitedRow[4]
                        splitedRow.pop(3)
                        splitedRow.pop(3)
                    else:
                        splitedRow[2] = splitedRow[2] + splitedRow[3]
                        splitedRow.pop(3)

                    writer.writerow(splitedRow)
                    print(splitedRow)

                    
def column_parsing_statistics(file_to_read, file_to_write):
    with open(file_to_read, "r") as file:
        reader = csv.reader(file)

        with open(file_to_write, "w") as target_file:
            writer = csv.writer(target_file)

            for row in reader:
                writer.writerow(row)
                

if __name__ == "__main__": 
    if (len(sys.argv) == 5):
        if ("all" in sys.argv[4]):
            column_parsing_all(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
        elif ("listen" in sys.argv[4]):
            column_parsing_listen(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    elif (len(sys.argv) == 4):
        print("asdf")
        if ("statistic" in sys.argv[3]):
            print("asdfas")
            column_parsing_statistics(sys.argv[1], sys.argv[2])
    else:
        print("Argument error!")
