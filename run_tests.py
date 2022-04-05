import unittest


class LocalTestRunner:
    def run_tests():
        loader = unittest.TestLoader()
        tests = loader.discover(".", "test*.py")
        testRunner = unittest.runner.TextTestRunner()
        results = testRunner.run(tests)
        return results


def main():
    results = LocalTestRunner.run_tests()
    if results.wasSuccessful():
        exit(0)
    else:
        exit(1)


if __name__ == "__main__":
    main()
