from behave import __main__ as behave_main
from os import path

if __name__ == "__main__":
    # Run the behave tests

    feature_directory = path.join(path.dirname(__file__), "features")  # Adjust this path as necessary
    print(feature_directory)
    behave_main.main([feature_directory])


        