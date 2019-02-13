def generateLine(token, output):
    token = "\""+token+"\""
    print("{0:<15}if (g_tokens){{printf(\"{1}\\n\");}} return {1};".format(token, output))

if __name__ == "__main__":
    var = input("")
    var2 = input("")
    while(var != "exit"):
        generateLine(var, var2)
        var = input("")
        var2 = input("")
