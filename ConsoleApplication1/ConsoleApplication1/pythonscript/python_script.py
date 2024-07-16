import subprocess

def run_program(inputs):
    print("Running program with inputs:")
    print(inputs)

    process = subprocess.Popen(
        ['D:\\paradigms\\assigment5\\assigment5_paradigms\\ConsoleApplication1\\x64\\Debug\\ConsoleApplication1.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input=inputs)
    return stdout.strip(), stderr.strip()

def test_program():
    tests = [
        {
            "input": "2+2",
            "expected_output": "Result: 4"
        },
        {
            "input": "min(2,3)",
            "expected_output": "Result: 2"
        },
        {
            "input": "max(2,3)",
            "expected_output": "Result: 3"
        },
        {
            "input": "pow(2,11)",
            "expected_output": "Result: 2048"
        },
        {
            "input": "b = 1+1",
            "expected_output": "b = 2"
        },
        {
            "input": "b + min(4,max(1,2))",
            "expected_output": "Result: 4"
        },
        {
            "input": "abs(b - 3)",
            "expected_output": "Result: 1"
        },
        {
            "input": "a + 1",
            "expected_output": "Result: 2"
        },

        {
            "input": "min(2,max(2,3))",
            "expected_output": "Result: 2"
        },
        {
            "input": "mi(2,max(2,3))",
            "expected_output": "Result: 2"
        }

    ]

    all_inputs = "\n".join([test["input"] for test in tests]) + "\nexit\n"
    output, error = run_program(all_inputs)
    output_lines = output.split("\n")

    for i, test in enumerate(tests):
        input_data = test["input"]
        expected_output = test["expected_output"]
        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif expected_output not in output_lines[i]:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output_lines[i]}")
        else:
            print(f"Test passed for input:\n{input_data}")

if __name__ == "__main__":
    test_program()
