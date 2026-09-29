#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <iostream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

int main() {
	// Load the image as an Eigen matrix with size m x n. Each entry in the matrix corresponds
	// to a pixel on the screen and takes a value somewhere between 0 (black) and 255 (white).
	// Report the size of the matrix.

	const char* input_image_path = "res/deer.jpg";

	// Load the image using stb_image
	int m, n, channels;
	
	uint8_t* image_data = stbi_load(input_image_path, &m, &n, &channels, 1);
	if (!image_data) {
		std::cerr << "Error: Could not load image " << input_image_path << std::endl;
		return 1;
	}

	auto original_image = Map<Matrix<uint8_t, Dynamic, Dynamic, RowMajor>>(image_data, n, m);

	std::cout << "Task 1: Image loaded in Eigen::Matrix with size " << original_image.cols() << " x " << original_image.rows() << std::endl;

	// Introduce a noise signal into the loaded image by adding random fluctuations of color
	// ranging between [−50, 50] to each pixel. Export the resulting image in .png and upload it.

	Matrix<double, Dynamic, Dynamic, RowMajor> noisy_image = (original_image.cast<double>() + (MatrixXd::Random(m, n) * 50)).cwiseMax(0.0).cwiseMin(255.0);
	Matrix<uint8_t, Dynamic, Dynamic, RowMajor> save_noisy_image = noisy_image.cast<uint8_t>();

	const char* output_image_path = "res/noisy.png";
  	if (stbi_write_png(output_image_path, m, n, 1, save_noisy_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		return 1;
	}

	std::cout << "Task 2: Image saved to " << output_image_path << std::endl;

	// Reshape the original and noisy images as vectors v and w, respectively. Verify that each
	// vector has m n components. Report here the Euclidean norm of v.

	auto v = Map<const Matrix<uint8_t, Dynamic, 1>>(original_image.data(), original_image.size()).cast<double>();
	auto w = Map<const Matrix<double, Dynamic, 1>>(noisy_image.data()	 , noisy_image	 .size());

	std::cout << "Task 3: Euclidean norm of v ||v||=" << v.norm() << std::endl;

	// Write the convolution operation corresponding to the smoothing kernel Hav1 as a matrix
	// vector multiplication between a matrix A1 having size mn x mn and the image vector.
	// Report the number of non-zero entries in A1.

	size_t A1_size = m*n;
	SparseMatrix<double> A1(A1_size, A1_size);

	A1.reserve(VectorXi::Constant(A1_size,9)); // The 3x3 filter requires 9 nnz entries for each column except for pixels at the borders that require less

	for(int i = 0; i < A1_size; i++) {
		int row = i / n;
		int col = i % n;

		bool firstCol = (col == 0);
		bool lastCol  = (col == m-1);
		bool firstRow = (row == 0);
		bool lastRow  = (row == n-1);

		A1.insert(i,i) = 4;
		if(!firstCol) {
			A1.insert(i, i-1) = 1;
			if(!firstRow) {
				A1.insert(i, i-1-n) = 1;
			}
			if(!lastRow) {
				A1.insert(i, i-1+n) = 1;
			}
		}
		if(!lastCol) {
			A1.insert(i, i+1) = 1;
			if(!firstRow) {
				A1.insert(i, i+1-n) = 1;
			}
			if(!lastRow) {
				A1.insert(i, i+1+n) = 1;
			}
		}
		if(!firstRow) {
			A1.insert(i, i-n) = 1;
		}
		if(!lastRow) {
			A1.insert(i, i+n) = 1;
		}
	}
	A1 /= 12;

	std::cout << "Task 4: nnz(A1)=" << A1.nonZeros() << std::endl;
	// Apply the previous smoothing filter to the noisy image by performing the matrix vector
	// multiplication A1w. Export and upload the resulting image.

	Vector<double, Dynamic> smoothed_noisy = A1*w;
	Vector<uint8_t, Dynamic> save_image = smoothed_noisy.cast<uint8_t>();

	const char* smoothed_noisy_image_path = "res/smoothed_noisy.png";
  	if (stbi_write_png(smoothed_noisy_image_path, m, n, 1, save_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		return 1;
	}
	std::cout << "Task 5: Image saved to " << smoothed_noisy_image_path << std::endl;

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
