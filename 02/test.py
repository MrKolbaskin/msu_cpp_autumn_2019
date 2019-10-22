import subprocess
import time

def run(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = process.stdout.readlines()
    code = process.wait()
    return code, out

def test(command, expected_code, expected_value):
    print (command)
    code, out = run(command)
    if code != expected_code:
        print 'return value', expected_code, '(expected) !=', code
        return
    i = 0
    for line in out:
        try:
            if line.rstrip() != expected_value[i]:
                print expected_value[i], '(expected) !=', line.rstrip()
                return
            i += 1
        except ValueError:
            print 'invalid output'
            return
        except IndexError:
            print 'invalid output'
            return
    if i != len(expected_value):
        print 'empty output'
        return
    print 'ok'

test('./my_prog 100 alloc 1000', 1, [ 'Bad alloc' ])
test('./my_prog test', 1, [ 'Wrong input' ])
test('./my_prog 100 alloc 10 alloc 90 alloc 10', 1, [ 'Bad alloc' ])
test('./my_prog 100 alloc 10 alloc 15 reset alloc 90', 0, [])
test('./my_prog 1000 alloc 1000 alloc 15 reset alloc 90', 1, ['Bad alloc'])
test('./my_prog 100 alloc test', 1, ['Wrong input'])
test('./my_prog reset', 1, ['Wrong input'])
test('./my_prog 1 reset reset alloc 1 reset alloc 10', 1, ['Bad alloc'])

