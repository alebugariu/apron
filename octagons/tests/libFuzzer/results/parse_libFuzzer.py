#!/usr/bin/python3

import csv
import sys

def dump_result(writer, results):
    if results is not None:
        print(results)
        writer.writerow([results[column] for column in columns])

if len(sys.argv) != 3:
    print("{} input output".format(sys.argv[0]))
    sys.exit(1)
else:
    _, input_file, output_file = sys.argv

with open(output_file, 'w') as results_fp:
    writer = csv.writer(results_fp)
    columns = [
        'test case',
        'execution time (seconds)',
        'execution time (minutes)',
        'initial coverage',
        'final coverage',
        'status',
        'error message'
    ]
    writer.writerow(columns)
    for test_file in [input_file]:
        results = {}
        results['error message']='-'
        results['status']='PASS'
        with open(test_file) as fp:
            last_line = None
            for line in fp:
                if line.startswith('#'):
                   last_line = line
                elif last_line is not None:
                   cov = last_line.split('cov: ')[1].split()[0]
                   results['final coverage']=int(cov.strip())
                if line.startswith('#') and 'INITED' in line:
                   cov = line.split('cov: ')[1].split()[0]
                   results['initial coverage']=int(cov.strip())
 		elif 'Killed' in line:
                   results['status']='KILLED'  
                elif line.startswith('Execution time: '):
                   line = line.replace('Execution time: ', '').strip();
                   values = line.split(" ");
                   results['execution time (seconds)'] = int(values[0])
                   results['execution time (minutes)'] = float(values[0]) / 60
                   results['test case'] = values[-1]
                   dump_result(writer, results)
                   results = {}
                   results['initial coverage']= '-'
                   results['final coverage']= '-'
		   results['error message']='-'
                   results['status']='PASS'
