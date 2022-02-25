# Python program to print
# colored text and background
class Colors:
    '''
    Colors class:reset all Colors with Colors.reset; two
    sub classes fg for foreground
    and bg for background; use as Colors.subclass.colorname.
    i.e. Colors.fg.red or Colors.bg.greenalso, the generic bold, disable,
    underline, reverse, strike through,
    and invisible work with the main class i.e. Colors.bold\
    '''
    reset='\033[0m'
    bold='\033[01m'
    disable='\033[02m'
    underline='\033[04m'
    reverse='\033[07m'
    strikethrough='\033[09m'
    invisible='\033[08m'
    class fg:
        black='\033[30m'
        red='\033[31m'
        green='\033[32m'
        orange='\033[33m'
        blue='\033[34m'
        purple='\033[35m'
        cyan='\033[36m'
        lightgrey='\033[37m'
        darkgrey='\033[90m'
        lightred='\033[91m'
        lightgreen='\033[92m'
        yellow='\033[93m'
        lightblue='\033[94m'
        pink='\033[95m'
        lightcyan='\033[96m'
    class bg:
        black='\033[40m'
        red='\033[41m'
        green='\033[42m'
        orange='\033[43m'
        blue='\033[44m'
        purple='\033[45m'
        cyan='\033[46m'
        lightgrey='\033[47m'

    def makeRed(input:str):
        return f'{Colors.fg.red}{input}{Colors.reset}'

    def makeGreen(input:str):
        return f'{Colors.fg.green}{input}{Colors.reset}'

    def makeOrange(input:str):
        return f'{Colors.fg.orange}{input}{Colors.reset}'

    def makeBlue(input:str):
        return f'{Colors.fg.blue}{input}{Colors.reset}'

    def makePurple(input:str):
        return f'{Colors.fg.purple}{input}{Colors.reset}'

    def makeCyan(input:str):
        return f'{Colors.fg.cyan}{input}{Colors.reset}'

    def makeLightGrey(input:str):
        return f'{Colors.fg.lightgrey}{input}{Colors.reset}'

    def makeDarkGrey(input:str):
        return f'{Colors.fg.darkgrey}{input}{Colors.reset}'
