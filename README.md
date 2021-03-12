# project-1
File Wrapping Project

Janhavi Narain      jn579

Priya Patel         prp78

When we tested our code, we looked at each of the three versions individually. First, when we tested the input to standard input, we tested with a width of 5 and a simple line like "Hi my name is Priya". When that worked, we tested that same line with a width of 10, 50, and 2. Then, we made a few sample text files with paragraphs in order to test that the only number of \n new lines written were 0, 1, or 2. We tested those under standard input with the same widths. We also tested with a bunch of blank lines before, in between, and at the end of the input to make sure it took care of those. Once we were certain that our wrapping method worked, we looked at doing the same thing but with the input of a file instead of standard input. We tested with the same text files as before as well as the initial single line test case. Once again, we tested with the same widths and new line testcases, we looked into the directory portion. We tested first with a directory we made containing the text files. Once we were sure that worked, we retested it in the same directory to make sure it didn't wrap the wrapped files. Then, we added an empty directory in the directory and a directory with a random text file inside. When we were sure we were getting the correct outputs, we tested with the empty directory as the input to make sure it worked fine.
