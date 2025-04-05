from behave import __main__ as behave_main

if __name__ == "__main__":
    # Run the behave tests
    behave_main.main()
    # You can specify the path to your features directory if needed
    # behave_main.main(args=["-i", "features"])
    # Or use the default path
    # behave_main.main(args=["features"])
    # Or run specific feature files
    # behave_main.main(args=["features/your_feature_file.feature"])
    # Or run specific scenarios
    # behave_main.main(args=["features/your_feature_file.feature:line_number"])
    # Or run specific tags
    # behave_main.main(args=["--tags=@your_tag"])
    # Or run all tests
    # behave_main.main(args=["features"])