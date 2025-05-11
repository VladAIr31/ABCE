import random
import sys
import os

MIN_VAL = -1000000000
MAX_VAL = 1000000000

OP_INSERT = 1
OP_DELETE = 2
OP_CHECK = 3
OP_FIND_MAX_LE_X = 4
OP_FIND_MIN_GE_X = 5
OP_DISPLAY_SORTED_RANGE = 6

def generate_random_number():
    return random.randint(MIN_VAL, MAX_VAL)

def generate_and_write_tests(num_total_operations, percentages, output_file_handle):

    numeric_percentages = {k: v for k, v in percentages.items() if isinstance(v, (int, float))}
    current_sum_percentages = sum(numeric_percentages.values())
    if abs(current_sum_percentages - 100.0) > 1e-9: # Toleranță pentru float
        print(f"Atenție: Suma procentelor numerice ({current_sum_percentages}) nu este exact 100.")


    operations_to_plan = []
    for op_type, percentage in percentages.items():
        if not isinstance(percentage, (int, float)) or percentage < 0:
            continue
        count = round(num_total_operations * (percentage / 100.0))
        operations_to_plan.extend([op_type] * int(count))


    current_total_planned = len(operations_to_plan)
    if current_total_planned < num_total_operations:
        possible_ops = [op for op, perc in percentages.items() if isinstance(perc, (int,float)) and perc > 0]
        if not possible_ops:
            possible_ops = [op for op in percentages.keys() if isinstance(percentages.get(op), (int,float))]

        while len(operations_to_plan) < num_total_operations and possible_ops:
            operations_to_plan.append(random.choice(possible_ops))
    elif current_total_planned > num_total_operations:
        while len(operations_to_plan) > num_total_operations:
            operations_to_plan.pop(random.randrange(len(operations_to_plan)))

    random.shuffle(operations_to_plan)

    simulated_set = set()

    # Scrie numărul total de operații (efectiv planificate)
    output_file_handle.write(f"{len(operations_to_plan)}\n")

    actual_operations_written = 0
    for i, op_type in enumerate(operations_to_plan):
        line_to_write = ""
        if op_type == OP_INSERT:
            x = generate_random_number()
            line_to_write = f"{OP_INSERT} {x}"
            simulated_set.add(x)
        elif op_type == OP_DELETE:
            x = generate_random_number()
            line_to_write = f"{OP_DELETE} {x}"
            simulated_set.discard(x)
        elif op_type == OP_CHECK:
            x = generate_random_number()
            line_to_write = f"{OP_CHECK} {x}"
        elif op_type == OP_FIND_MAX_LE_X:
            x = generate_random_number()
            line_to_write = f"{OP_FIND_MAX_LE_X} {x}"
        elif op_type == OP_FIND_MIN_GE_X:
            x = generate_random_number()
            line_to_write = f"{OP_FIND_MIN_GE_X} {x}"
        elif op_type == OP_DISPLAY_SORTED_RANGE:
            x = generate_random_number()
            y = generate_random_number()
            if random.random() < 0.8:
                if x > y:
                    x, y = y, x
            line_to_write = f"{OP_DISPLAY_SORTED_RANGE} {x} {y}"
        else:
            continue

        output_file_handle.write(line_to_write + "\n")
        actual_operations_written +=1

        if (i + 1) % 1_000_000 == 0:
            print(f"Generat și scris {i + 1}/{len(operations_to_plan)} operații...")

    return actual_operations_written


if __name__ == "__main__":
    Q = 1000000

    output_directory = "gen"
    base_filename = "eq.in"


    full_output_path = os.path.join(output_directory, base_filename)

    operation_percentages = {
        OP_INSERT: 17,
        OP_DELETE: 16.6,
        OP_CHECK: 16.6,
        OP_FIND_MAX_LE_X: 16.6,
        OP_FIND_MIN_GE_X: 16.6,
        OP_DISPLAY_SORTED_RANGE:16.6
    }

    current_sum_percentages = sum(p for p in operation_percentages.values() if isinstance(p, (int, float)))
    if abs(current_sum_percentages - 100.0) > 1e-9:
        print(f"Eroare critică: Suma procentelor ({current_sum_percentages}) nu este 100. Verifică configurația.")
    else:
        print(f"Suma procentelor configurate: {current_sum_percentages}")

        try:

            os.makedirs(output_directory, exist_ok=True)
            print(f"Directorul de output: {os.path.abspath(output_directory)}") # Afișează calea absolută
            print(f"Se vor genera aproximativ {Q} operații în fișierul: {full_output_path}")

            with open(full_output_path, 'w') as f:
                num_written = generate_and_write_tests(Q, operation_percentages, f)

            print(f"Testele au fost generate cu succes în fișierul: {full_output_path}")
            print(f"Numărul de operații efectiv scrise: {num_written}")

        except ValueError as e:
            print(f"Eroare la generarea testelor: {e}")
        except OSError as e:
            print(f"Eroare de sistem (de ex., legată de fișiere/directoare): {e}")
        except Exception as e:
            print(f"A apărut o eroare neașteptată: {e}")