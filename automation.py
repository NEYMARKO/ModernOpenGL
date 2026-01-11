import re
import time
import subprocess
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt

WAIT_TIME = 15
RAGDOLLS_PER_ITERATION = 2

def get_varying_line_idx(file_content : list[str], keyword : str) -> int:
    for i in range(len(file_content)):
        if keyword in file_content[i]:
            return i
    return -1

def modify_line(original_line : str, new_value : str) -> str:
    modified_line = ""
    print(f"{original_line=}")
    original_value = re.search(r"(?<=\=)\s*([^;]+)\s*(?=;)", original_line).group(1)
    start = original_line.find(original_value)
    modified_line = original_line[:start] + new_value + ";\n"
    print(f"{modified_line=}")
    return modified_line

def modify_source_file(file_path : str, line_idx : int, new_value : int, lines : list[str]) -> None:
    file = open(file_path, "w")
    lines[line_idx] = modify_line(lines[line_idx], str(new_value))
    # print(f"modified lines: {lines}")
    file.writelines(lines)
    file.close()
    return 

def get_file_content(source_file_path : Path) -> list[str]:
    lines = []
    with open(source_file_path, "r") as f:
        lines = f.readlines()
    return lines

def process_mid_results(results : list[str]) -> float:
    total = 0
    print(f"{results=}")
    for r in results:
        total += float(r)
    return total / len(results)

def plot_results(final_results : dict[int, float]) -> None:
    x = np.array(list(final_results.keys()))
    y = np.array(list(final_results.values()))
    plt.plot(x, y)
    plt.show()
    return

def main():
    script_path = Path()
    build_command = f"devenv Engine.sln /Build"
    project_folder_path = script_path / "Engine"
    result = subprocess.run(build_command, 
                            shell=True, 
                            capture_output=True, 
                            text=True,
                            cwd=project_folder_path)
    exe_path = project_folder_path / "x64/Debug/Engine.exe"
    target_file_path = str((project_folder_path / "Main.cpp").absolute())
    print(f"{target_file_path=}")
    source_file_content = get_file_content(source_file_path=target_file_path)
    idx = get_varying_line_idx(source_file_content, "ragdoll_count")
    quit_signal_path = Path(script_path / "quit.flag")
    quit_signal_path.touch()

    final_results ={}
    for i in range(1, 40):
        quit_signal_path.unlink()
        modify_source_file(target_file_path, idx, i * RAGDOLLS_PER_ITERATION, source_file_content)
        subprocess.run(build_command, 
                                shell=True, 
                                capture_output=True, 
                                text=True,
                                cwd=project_folder_path)
        proc = subprocess.Popen(
            exe_path,
            cwd=exe_path.parent,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        time.sleep(WAIT_TIME)
        quit_signal_path.touch()

        stdout, _ = proc.communicate()
        print(f"{stdout=}")
        final_results[i * RAGDOLLS_PER_ITERATION] = process_mid_results(stdout.strip().split("\n"))
    plot_results(final_results)
    # print(f"{final_results=}")
    return

if __name__ == "__main__":
    main()