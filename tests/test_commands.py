import unittest

from src.commands import Commands


class CommandsTestCase(unittest.TestCase):
    def test_num_to_uni_simple(self):
        stri = "0000"
        stro = "\u23F4\u23F4\u23F4\u23F4"
        test = Commands.num_to_unicode(stri)
        self.assertEqual(stro, test)

    def test_uni_to_num_simple(self):
        stri = "0000"
        stro = "\u23F4\u23F4\u23F4\u23F4"
        test = Commands.unicode_to_num(stro)
        self.assertEqual(stri, test)

    def test_num_to_uni_allfour(self):
        stri = "01231302"
        stro = "\u23F4\u23F5\u23F6\u23F7\u23F5\u23F7\u23F4\u23F6"
        test = Commands.num_to_unicode(stri)
        self.assertEqual(stro, test)

    def test_uni_to_num_allfour(self):
        stri = "01231302"
        stro = "\u23F4\u23F5\u23F6\u23F7\u23F5\u23F7\u23F4\u23F6"
        test = Commands.unicode_to_num(stro)
        self.assertEqual(stri, test)


if __name__ == '__main__':
    unittest.main()
