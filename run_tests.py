import unittest

loader = unittest.TestLoader()
tests = loader.discover('.', 'test*.py')
testRunner = unittest.runner.TextTestRunner()
results = testRunner.run(tests)
if results.wasSuccessful():
    exit(0)
else:
    exit(1)