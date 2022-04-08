from bidict import bidict


class Commands:

    # Triangle version
    _cmd_map = bidict(
        {
            "0": "\u23F4",  # left      # left
            "1": "\u23F5",  # right     # right
            "2": "\u23F6",  # up        # jump
            "3": "\u23F7",  # down      # toggle object
        }
    )

    def num_to_unicode(cmdstr: str) -> str:
        """
        Converts a command string from numbers to unicode.

        ### Parameters
        `cmdstr : str`:
            The command string, in command numbers

        ### Returns
        `str`
        """
        newstr = ""
        for c in cmdstr:
            newstr += Commands._cmd_map[c]

        return newstr

    def unicode_to_num(cmdstr: str) -> str:
        """
        Converts a command string from numbers to unicode.

        ### Parameters
        `cmdstr : str`:
            The command string, in unicode

        ### Returns
        `str`
        """
        newstr = ""
        for c in cmdstr:
            newstr += Commands._cmd_map.inverse[c]

        return newstr
