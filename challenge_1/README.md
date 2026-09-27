# NLA - Challenge 1
**Goal**: Apply image filters and find the approximate solution of linear system to process a
greyscale image.

**Data**: Download the file deer.jpg from webeep and move it to your working directory.

**Tasks**:
1. **Q**: Load the image as an Eigen matrix with size $m \times n$. Each entry in the matrix corresponds to a pixel on the screen and takes a value somewhere between 0 (black) and 255 (white). Report the size of the matrix.

   **A**: 

2. **Q**: Introduce a noise signal into the loaded image by adding random fluctuations of color ranging between $[−50, 50]$ to each pixel. Export the resulting image in **.png** and upload it.

   **A**: 

3. **Q**: Reshape the original and noisy images as vectors $v$ and $w$, respectively. Verify that each vector has m n components. Report here the Euclidean norm of $v$.

   **A**: 

4. **Q**: Write the convolution operation corresponding to the smoothing kernel $H_{av1}$ as a matrix vector multiplication between a matrix $A_1$ having size $mn \times mn$ and the image vector. Report the number of non-zero entries in $A_1$.

   **A**: 

5. **Q**: Apply the previous smoothing filter to the noisy image by performing the matrix vector multiplication $A_{1}w$. Export and upload the resulting image.

   **A**: 

6. **Q**: Write the convolution operation corresponding to the sharpening kernel $H_{sh1}$ as a matrix vector multiplication by a matrix $A_2$ having size $mn \times mn$. Report the number of non-zero entries in $A_2$. Is $A_2$ symmetric?

   **A**: 

7. **Q**: Apply the previous sharpening filter to the original image by performing the matrix vector multiplication $A_2 v$. Export and upload the resulting image.

   **A**: 

8. **Q**: Export the Eigen matrix $A_2$ and vector win the **.mtx** format. Using a suitable iterative solver and preconditioning technique available in the LIS library compute the approximate solution to the linear system $A_2 x = w$ prescribing a tolerance of $10^{−12}$. Report here the iteration count and the final residual.

   **A**: 

9. **Q**: Convert the previous approximate solution vector xinto a **.png** image. Upload the resulting file here.

   **A**: 

10. **Q**: Write the convolution operation corresponding to the detection kernel $H_{ed2}$ as a matrix vector multiplication by a matrix $A_3$ having size $mn \times mn$. Is matrix $A_3$ symmetric?

    **A**: 

11. **Q**: Apply the previous edge detection filter to the original image by performing the matrix vector multiplication $A_3 v$. Export and upload the resulting image.

    **A**: 

12. **Q**: Using a suitable iterative solver available in the Eigen library compute the approximate solution of the linear system $(4I + A_3)y = w$, where $I$ denotes the identity matrix, prescribing a tolerance of $10^{−10}$. Report here the iteration count and the final residual.

    **A**: 

13. **Q**: Import the previous approximate solution vector $y$ in Eigen and convert it into a **.png** image and upload it.

    **A**: 
