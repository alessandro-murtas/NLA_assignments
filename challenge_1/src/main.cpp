#include <Eigen>

int main() {
	// Load the image as an Eigen matrix with size m x n. Each entry in the matrix corresponds
	// to a pixel on the screen and takes a value somewhere between 0 (black) and 255 (white).
	// Report the size of the matrix.

	// Introduce a noise signal into the loaded image by adding random fluctuations of color
	// ranging between [−50, 50] to each pixel. Export the resulting image in .png and upload it.

	// Reshape the original and noisy images as vectors vand w, respectively. Verify that each
	// vector has m n components. Report here the Euclidean norm of v.

	// Write the convolution operation corresponding to the smoothing kernel Hav1 as a matrix
	// vector multiplication between a matrix A1 having size mn ×mn and the image vector.
	// Report the number of non-zero entries in A1.

	// Apply the previous smoothing filter to the noisy image by performing the matrix vector
	// multiplication A1w. Export and upload the resulting image.

	// Write the convolution operation corresponding to the sharpening kernel Hsh1 as a matrix
	// vector multiplication by a matrix A2 having size mn x mn. Report the number of non-zero
	// entries in A2. Is A2 symmetric?

	// Apply the previous sharpening filter to the original image by performing the matrix vector
	// multiplication A2v. Export and upload the resulting image.

	// Export the Eigen matrix A2 and vector win the .mtx format. Using a suitable iterative
	// solver and preconditioning technique available in the LIS library compute the approximate
	// solution to the linear system A2x= wprescribing a tolerance of 10−12. Report here the
	// iteration count and the final residual.

	// Convert the previous approximate solution vector xinto a .png image. Upload the resulting
	// file here.

	// Write the convolution operation corresponding to the detection kernel Hed2 as a matrix
	// vector multiplication by a matrix A3 having size mn ×mn. Is matrix A3 symmetric?

	// Apply the previous edge detection filter to the original image by performing the matrix
	// vector multiplication A3 v. Export and upload the resulting image.

	// Using a suitable iterative solver available in the Eigen library compute the approximate
	// solution of the linear system (4I +A3)y= w, where I denotes the identity matrix, prescribing 
	// a tolerance of 10−10. Report here the iteration count and the final residual.

	// Import the previous approximate solution vector yin Eigen and convert it into a .png
	// image and upload it.
	return 0;
}
