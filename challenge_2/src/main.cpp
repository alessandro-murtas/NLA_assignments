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

std::vector<double> nBiggestEigenvaluesSymmetricMatrix(MatrixXd m, unsigned n) {
    SelfAdjointEigenSolver<MatrixXd> eigensolver(m);

    if (eigensolver.info() != Eigen::Success) {
        std::cerr << "Error: Could not compute eigensolver!" << std::endl;
        abort();
    }

    std::vector<double> nBiggestEigenvalues;

    auto eigenvalues = eigensolver.eigenvalues();
    for(int i = 0; i < n; i++) {
        nBiggestEigenvalues.push_back(eigenvalues[m.cols() - 1 - i]);
    }

    return nBiggestEigenvalues;
}

int main(int argc, char *argv[]) {
    const char *input_image_path = argc > 1 ? argv[1] : "res/deer.jpg";

    // TASK 1

    // Load the image using stb_image
    int width, height, channels;
    uint8_t *image_data = stbi_load(input_image_path, &width, &height, &channels, 1); 
    if (!image_data){
        std::cerr << "Error: Could not load image " << input_image_path << std::endl;
        return 1;
    }

    // Prepare Eigen matrices for each image
    MatrixXd original_matrix(height, width);

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int index = (i * width + j);

            original_matrix(i, j) = static_cast<double>(image_data[index])/255.0;
        }
    }

    MatrixXd matrix_ata = original_matrix.transpose()*original_matrix;

    std::cout << "Task 1: ||A^T*A|| = " << matrix_ata.norm() << std::endl;

    // TASK 2
    auto nBiggestEigenvalues = nBiggestEigenvaluesSymmetricMatrix(matrix_ata, 2);
    
    std::cout << "The 2 biggest eigenvalues of A are: " << sqrt(nBiggestEigenvalues[0]) << ", " << sqrt(nBiggestEigenvalues[1]) << std::endl;

    // TASK 3
    saveMarket(matrix_ata, "res/ATA.mtx");

    std::string command = "./eigen1 ATA.mtx eigvec.txt hist.txt -e pi -etol 1.0e-10";

	std::cout << std::endl << command << std::endl;
	int result = system(("cd res && " + command).c_str());

	if (result) {
		std::cout << "Failed command execution" << std::endl;
		return 1;
	}

    // TASK 4
    command = "./eigen1 ATA.mtx eigvec.txt hist.txt -e pi -etol 1.0e-10 -shift 867.85";

	std::cout << std::endl << command << std::endl;
	result = system(("cd res && " + command).c_str());

	if (result) {
		std::cout << "Failed command execution" << std::endl;
		return 1;
	}

    // TASK 5
    BDCSVD<MatrixXd> svd(original_matrix, ComputeThinU | ComputeThinV);

    MatrixXd sigma = svd.singularValues().asDiagonal();
    
    std::cout << "Task 5: ||Σ|| = " << sigma.norm() << std::endl;

    // TASK 6
    MatrixXd C30 = svd.matrixU().leftCols(30);
    MatrixXd C90 = svd.matrixU().leftCols(90);

    MatrixXd D30 = svd.matrixV().leftCols(30); // *sigma.topRows(30);
    MatrixXd D90 = svd.matrixV().leftCols(90); //*sigma.topRows(90);

    for(int i = 0; i < 30; i++) {
        D30.col(i) *= sigma(i,i);
    }


    for(int i = 0; i < 90; i++) {
        D90.col(i) *= sigma(i,i);
    }

    // TASK 6
    MatrixXd A30 = C30*D30.transpose();
    MatrixXd A90 = C90*D90.transpose();

    Matrix<uint8_t, Dynamic, Dynamic, RowMajor> save_image = (A30*255).cwiseMax(0.0).cwiseMin(255.0).cast<uint8_t>();
    const char* smoothed_noisy_image_path = "res/A30.png";
  	if (stbi_write_png(smoothed_noisy_image_path, width, height, 1, save_image.data(), width) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
		//NB: the last parametere m indices how many bytes per row, 
		//since we have single channel and uint8_t, we have 1 byte per pixel, so m bytes per row

	}
	std::cout << "Task 6: Image saved to " << smoothed_noisy_image_path << std::endl;
    const char* smoothed_noisy_image_path90 = "res/A90.png";
    save_image = (A90*255).cwiseMax(0.0).cwiseMin(255.0).cast<uint8_t>();
  	if (stbi_write_png(smoothed_noisy_image_path90, width, height, 1, save_image.data(), width) == 0) {
    	std::cerr << "Error: Could not save grayscale image" << std::endl;
		stbi_image_free(image_data);
		return 1;
		//NB: the last parametere m indices how many bytes per row, 
		//since we have single channel and uint8_t, we have 1 byte per pixel, so m bytes per row
	}
	std::cout << "Task 6: Image saved to " << smoothed_noisy_image_path90 << std::endl;
}