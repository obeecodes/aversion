#!/usr/bin/env fish

# Find all files in include and src directories, excluding the SDL2 directory and count lines
set include_dir include
set src_dir src

# Function to count lines of code in a directory excluding SDL2
function count_lines
    set dir $argv[1]
    find $dir -type f -not -path "*/SDL2/*" | xargs wc -l | grep total | awk '{print $1}'
end

# Calculate lines of code in include and src directories
set include_lines (count_lines $include_dir)
set src_lines (count_lines $src_dir)

# Sum the lines
set total_lines (math $include_lines + $src_lines)

# Output the results
echo "Lines of code in include directory (excluding SDL2): $include_lines"
echo "Lines of code in src directory: $src_lines"
echo "Total lines of code: $total_lines"
