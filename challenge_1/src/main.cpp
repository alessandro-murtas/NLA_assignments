#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <unsupported/Eigen/SparseExtra>

#include <iostream>
#include <fstream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

// Hav1 << 1,1,1,1,4,1,1,1,1; Hav1 /= 12;
// Hav2 << 0,1,2,1,0,1,4,8,4,1,2,8,16,8,2,1,4,8,4,1,0,1,2,1,0; Hav2 /= 80;
// Hsh1 << 0,-3,0,-1,9,-3,0,-1,0;
// Hed1 << 0,-1,0,-1,4,-1,0,-1,0;
// Hed2 << -1,0,1,-2,0,2,-1,0,1;

template<typename T>
SparseMatrix<T> createConvolutionMatrix(Matrix<T, Dynamic, Dynamic, RowMajor> kernel, size_t width, size_t height) {
	size_t size = width*height;
	SparseMatrix<T> convMatrix(size, size); //mn x mn

	int nnzKernel = 0;

	//Counting non-null kernel pixels
	for(int i = 0; i < kernel.rows(); i++) {
		for(int j = 0; j < kernel.cols(); j++) {
			if(kernel(i, j) != 0.0f) {
				nnzKernel++; //we count the number of non-zero elements in the kernel
				//we know on how many elements the pixel will depend
			}	
		}
	}
	
	convMatrix.reserve(VectorXi::Constant(size, nnzKernel)); //Prepares memory

	int rows = kernel.rows()/2; 
	int cols = kernel.cols()/2; 

	for(int n = 0; n < size; n++) {
		int col = n % width;
		int row = n / width;

		for(int i = -rows; i <= rows; i++) {
			for(int j = -cols; j <= cols; j++) {
				double value = kernel(i+rows, j+cols);
				if(row + i < 0 || col + j < 0 || row + i >= height || col + j >= width || value == 0) continue;
				
				int pos = n + j + i*width;
				
				convMatrix.insert(n, pos) = value;
			}
		}
	}

	return convMatrix;
}

template<typename T> // generic numeric type T for elements of the matrix
bool isSparseMatrixSymmetric(SparseMatrix<T> mat) {
	return (SparseMatrix<T>(mat.transpose()) - mat).norm() == 0; 
	//norma fra mat e la trasposta
}

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

	Matrix<double, Dynamic, Dynamic, RowMajor> noisy_image = (original_image.cast<double>() + (MatrixXd::Random(n, m) * 50)).cwiseMax(0.0).cwiseMin(255.0);
	Matrix<uint8_t, Dynamic, Dynamic, RowMajor> save_noisy_image = noisy_image.cast<uint8_t>();

	const char* output_image_path = "res/noisy.png";
  	if (stbi_write_png(output_image_path, m, n, 1, save_noisy_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
	}

	std::cout << "Task 2: Image saved to " << output_image_path << std::endl;

	// Reshape the original and noisy images as vectors v and w, respectively. Verify that each
	// vector has m n components. Report here the Euclidean norm of v.

	auto v = Map<const Matrix<uint8_t, Dynamic, 1>>(original_image.data(), original_image.size()).cast<double>();
	auto w = Map<const Matrix<double , Dynamic, 1>>(noisy_image.data()	 , noisy_image	 .size());

	std::cout << "Task 3: v.size() = " << v.size()
          << ", w.size() = " << w.size()
          << ", m*n = " << m*n << std::endl;

	std::cout << "Task 3: Euclidean norm of v ||v|| = "
          << v.norm() << std::endl;

	// Write the convolution operation corresponding to the smoothing kernel Hav1 as a matrix
	// vector multiplication between a matrix A1 having size mn x mn and the image vector.
	// Report the number of non-zero entries in A1.

	Matrix<double, Dynamic, Dynamic, RowMajor> Hav1(3, 3);

	Hav1 << 1, 1, 1,
			1, 4, 1,
			1, 1, 1;

	Hav1 /= 12;
	
	auto A1 = createConvolutionMatrix<double>(Hav1, m, n);

	std::cout << "Task 4: nnz(A1)=" << A1.nonZeros() << std::endl;
	// Apply the previous smoothing filter to the noisy image by performing the matrix vector
	// multiplication A1w. Export and upload the resulting image.

	VectorXd smoothed_noisy = A1*w;
	Matrix<uint8_t, Dynamic, Dynamic, RowMajor> save_image = smoothed_noisy.cast<uint8_t>();

	const char* smoothed_noisy_image_path = "res/smoothed_noisy.png";
  	if (stbi_write_png(smoothed_noisy_image_path, m, n, 1, save_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
		//NB: the last parametere m indices how many bytes per row, 
		//since we have single channel and uint8_t, we have 1 byte per pixel, so m bytes per row

	}
	std::cout << "Task 5: Image saved to " << smoothed_noisy_image_path << std::endl;

	// Write the convolution operation corresponding to the sharpening kernel Hsh1 as a matrix
	// vector multiplication by a matrix A2 having size mn x mn. Report the number of non-zero
	// entries in A2. Is A2 symmetric?

	Matrix<double, Dynamic, Dynamic, RowMajor> Hsh1(3, 3);

	Hsh1 <<  0, -3,  0,
			-1,  9, -3,
			 0, -1,  0;
			 
	auto A2 = createConvolutionMatrix<double>(Hsh1, m, n); //we create conv matrix for Hsh1

	std::cout << "Task 6: nnz(A2)=" << A2.nonZeros() << std::endl;
	std::cout << "Task 6: A2 is " << (isSparseMatrixSymmetric(A2) ? "" : "not ") << "symmetric" << std::endl;
	// Apply the previous sharpening filter to the original image by performing the matrix vector
	// multiplication A2v. Export and upload the resulting image.

	VectorXd sharpened_original = (A2*v).cwiseMax(0.0).cwiseMin(255.0);
	save_image = sharpened_original.cast<uint8_t>(); //cast because we want only integers

	const char* sharpened_original_image_path = "res/sharpened_original.png";
  	if (stbi_write_png(sharpened_original_image_path, m, n, 1, save_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
	}
	std::cout << "Task 7: Image saved to " << sharpened_original_image_path << std::endl;

	// Export the Eigen matrix A2 and vector w in the .mtx format. Using a suitable iterative
	// solver and preconditioning technique available in the LIS library compute the approximate
	// solution to the linear system A2x=w prescribing a tolerance of 10−12. Report here the
	// iteration count and the final residual.
	saveMarket(A2, "res/A2.mtx");
	// saveMarketVectorDense(w, "res/w.mtx");

	std::ofstream outW;
	outW.open("res/w.mtx");

	outW << "%%MatrixMarket vector coordinate real general\n";
    outW << w.rows() << "\n";

    for (int i=0; i < w.rows(); i++) {
		outW << i + 1 << " " << w(i) << "\n";
    }
	outW.close();

	std::string command = "./test1 A2.mtx w.mtx x.mtx hist.txt -tol 1.0e-12 -i bicgstab -p iluc";

	std::cout << std::endl << command << std::endl;
	int result = system(("cd res && " + command).c_str());

	if (result) {
		std::cout << "Failed command execution" << std::endl;
		return 1;
	}

	// std::cout << "Task 8: Iterations = 8, final residual = 8.151045e-14" << std::endl;
	// Convert the previous approximate solution vector x into a .png image. Upload the resulting
	// file here.
	std::ifstream inX;
	inX.open("res/x.mtx");

	std::string str;
	std::getline(inX, str);
	std::getline(inX, str);
	int vecSize = atoi(str.c_str());

	VectorXd x = VectorXd::Constant(vecSize, 0);
	int i = 0;
    while (std::getline(inX, str)) {
		std::istringstream s(str);

		s >> i;
		s >> x(i-1);
	}

	inX.close();

	x = x.cwiseMax(0.0).cwiseMin(255.0);
	save_image = x.cast<uint8_t>();

	const char* x_solution_image_path = "res/solution_x.png";
  	if (stbi_write_png(x_solution_image_path, m, n, 1, save_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		return 1;
	}
	std::cout << "Task 9: Image saved to " << x_solution_image_path << std::endl;

	// Write the convolution operation corresponding to the detection kernel Hed2 as a matrix
	// vector multiplication by a matrix A3 having size mn x mn. Is matrix A3 symmetric?

	Matrix<double, Dynamic, Dynamic, RowMajor> Hed2(3, 3);

	Hed2 << -1, 0, 1,
			-2, 0, 2,
			-1, 0, 1;
			 
	auto A3 = createConvolutionMatrix<double>(Hed2, m, n);

	std::cout << "Task 10: nnz(A3)=" << A3.nonZeros() << std::endl;
	std::cout << "Task 10: A3 is " << (isSparseMatrixSymmetric(A3) ? "" : "not ") << "symmetric" << std::endl;
	// Apply the previous edge detection filter to the original image by performing the matrix
	// vector multiplication A3 v. Export and upload the resulting image.

	VectorXd sobel_original = (A3*v).cwiseMax(0.0).cwiseMin(255.0);
	save_image = sobel_original.cast<uint8_t>();

	const char* sobel_original_image_path = "res/sobel_original.png";
  	if (stbi_write_png(sobel_original_image_path, m, n, 1, save_image.data(), m) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
	}
	std::cout << "Task 11: Image saved to " << sobel_original_image_path << std::endl;

	// Using a suitable iterative solver available in the Eigen library compute the approximate
	// solution of the linear system (4I +A3)y= w, where I denotes the identity matrix, prescribing 
	// a tolerance of 10−10. Report here the iteration count and the final residual.

	SparseMatrix<double> I(A3.rows(), A3.cols());
	I.setIdentity();

	SparseMatrix<double> system_matrix = 4.0*I + A3;

	BiCGSTAB<SparseMatrix<double>> solver;
	solver.setTolerance(1.e-10);
	solver.setMaxIterations(1000);

	solver.compute(system_matrix);

	VectorXd y = solver.solve(w);
	if (solver.info() != Eigen::Success) {
		std::cerr << "Error: Solver did not converge" << std::endl;
		stbi_image_free(image_data);
		return 1;
	}

	double relative_residual = (system_matrix*y - w).norm() / w.norm();

	std::cout << "Task 12: Iterations = " << solver.iterations()
			  << ", final residual = " << relative_residual << std::endl;

	// Import the previous approximate solution vector yin Eigen and convert it into a .png
	// image and upload it.

	VectorXd y_image = y.cwiseMax(0.0).cwiseMin(255.0);
	save_image = y_image.cast<uint8_t>();

	const char* solution_image_path = "res/solution_y.png";

	if (stbi_write_png(solution_image_path, m, n, 1, save_image.data(), m) == 0) {
		std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
	}

	std::cout << "Task 13: Image saved to " << solution_image_path << std::endl;
	stbi_image_free(image_data);
}
