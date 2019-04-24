# Sort Visualizer
Sort visualizer is a program to visualize various sorting algorithms.  
At the moment, it has these sorting algorithms implemented:  

- Selection Sort
- Insertion Sort
- Bubble Sort
- Merge Insertion Sort (Merge sort with insertion merging)
- Merge Sort (Top Down)
- Merge Sort (Bottom Up)

You can visualize the algorithms under the following views: 

- Rectangles - The height of the rectangle ∝ the value of the element
- Dots - The y value of the dot ∝ the value of the element
- Disparity dots - The further the element is from its sorted distance, the closer it is to 0. (Thanks to [w0rthy](https://www.youtube.com/watch?v=sYd_-pAfbBw) for the view)  
- Spiral - The radius of the dot ∝ the value of the element
- Circle - Each element gets a distinct color from the rainbow.

Currently there are 3 marks. Each of them visualizes different indices being used to access the array. (More marks may be added in the future as required by the algorithms)  
The audio is generated from a sine wave whose frequency ∝ the value of the element which is marked red.  

## Controls
- <kbd>W</kbd> and <kbd>S</kbd> to change the sorting algorithm. This only works when you're not sorting.  
- <kbd>A</kbd> and <kbd>D</kbd> to decrease and increase the element count (divide and multiply by 2). This also only works when you're not sorting.  
- <kbd>↑</kbd> and <kbd>↓</kbd> to increase and decrease the step delay (divide and multiply by 2).  
- <kbd>←</kbd> and <kbd>→</kbd> to change views.  

## Compilation
You will need the `clang` compiler, `SDL2`, `SDL2_image`, and [embed](https://github.com/wingerse/embed).  
`clang` and `embed` need to be in your path.  
### GNU/Linux
Do `make` to compile, `make RELEASE=1` to compile in release mode, `make run` to compile and run, and `make run RELEASE=1` to compile in release mode and run.  