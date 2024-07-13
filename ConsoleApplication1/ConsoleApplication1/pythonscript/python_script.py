import subprocess

def run_program(input_data, timeout=5):
    print(f"Running program with input: {input_data}")
    try:
        process = subprocess.Popen(
            ['D:\\paradigms\\assigment5\\assigment5_paradigms\\ConsoleApplication1\\ConsoleApplication1\\code.exe'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = process.communicate(input=input_data, timeout=timeout)
        return stdout.strip(), stderr.strip()
    except subprocess.TimeoutExpired:
        process.kill()
        stdout, stderr = process.communicate() 
        return stdout.strip(), stderr.strip()


def test_program():
    tests = [
        {
            "input": "2+2\n",
            "expected_output": "Result: 4"
        },
        {
            "input": "min(2,3)\n",
            "expected_output": "Result: 2"
        },
        {
            "input": "max(2,3)\n",
            "expected_output": "Result: 3"
        },
        {
            "input": "min(2,max(2,3))\n",
            "expected_output": "Result: 2"
        },
        {
            "input": "mi(2,max(2,3))\n",
            "expected_output": "Result: 2"
        }
    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_program(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif expected_output not in output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")

if __name__ == "__main__":
    test_program()



