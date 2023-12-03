
def inputHandler(filePath:str):
    ret = []
    with open(filePath, 'r') as file:
        for line in file:
            ret.append(line.strip())
    
    return ret


def extractor(line: str):
    current_num = ""
    for c in line:
        if c.isdigit():
            current_num += c

    num_str = current_num[0] + current_num[-1]
    return int(num_str)

def main():
    lines = inputHandler("./day1/input.txt")

    values = [extractor(line) for line in lines]

    # print(values)
    print(sum(values))


if __name__ == "__main__":
    main()