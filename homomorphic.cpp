#include <iostream>
#include <opencv2/opencv.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image) {
	Mat tmp, A, B, C, D;

	// se a imagem tiver tamanho impar, recorta a regiao para
	// evitar cópias de tamanho desigual
	image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
	int cx = image.cols / 2;
	int cy = image.rows / 2;

	// reorganiza os quadrantes da transformada
	// A B   ->  D C
	// C D       B A
	A = image(Rect(0, 0, cx, cy));
	B = image(Rect(cx, 0, cx, cy));
	C = image(Rect(0, cy, cx, cy));
	D = image(Rect(cx, cy, cx, cy));

	// A <-> D
	A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

	// C <-> B
	C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int, char**) {
	Mat imaginaryInput, complexImage, multsp;
	Mat padded, filter, mag;
	Mat image, imagegray, tmp;
	Mat_<float> realInput, zeros;
	vector<Mat> planos;

	image = imread("E:/Documents-bkp/UFRN/P8/processamento digital de imagens/site/images/homomorphic_filter/homomorphic_input.png");

	// valor do ruido
	float mean;

	// valores ideais dos tamanhos da imagem
	// para calculo da DFT
	int dft_M, dft_N;

	// identifica os tamanhos otimos para
	// calculo do FFT
	dft_M = getOptimalDFTSize(image.rows);
	dft_N = getOptimalDFTSize(image.cols);

	// realiza o padding da imagem
	copyMakeBorder(image, padded, 0,
		dft_M - image.rows, 0,
		dft_N - image.cols,
		BORDER_CONSTANT, Scalar::all(0));

	// parte imaginaria da matriz complexa (preenchida com zeros)
	zeros = Mat_<float>::zeros(padded.size());

	// prepara a matriz complexa para ser preenchida
	complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

	// a função de transferência (filtro frequencial) deve ter o
	// mesmo tamanho e tipo da matriz complexa
	filter = complexImage.clone();

	// cria uma matriz temporária para criar as componentes real
	// e imaginaria do filtro ideal
	tmp = Mat(dft_M, dft_N, CV_32F);

	float gama_h = 100;
	float gama_l = 510;
	float c = 10;
	float D_0 = 10;

	for (int i = 0; i<dft_M; i++) {
		for (int j = 0; j<dft_N; j++) {
			tmp.at<float>(i, j) = (gama_h - gama_l)*(1.0 - exp(-1.0*(float)c*((((float)i - dft_M / 2.0)*((float)i - dft_M / 2.0) + ((float)j - dft_N / 2.0)*((float)j - dft_N / 2.0)) / (D_0*D_0)))) + gama_l;
		}
	}

	// cria a matriz com as componentes do filtro e junta
	// ambas em uma matriz multicanal complexa
	Mat comps[] = { tmp, tmp };
	merge(comps, 2, filter);

	cvtColor(image, imagegray, CV_BGR2GRAY);
	imshow("original", imagegray);

	// realiza o padding da imagem
	copyMakeBorder(imagegray, padded, 0,
		dft_M - image.rows, 0,
		dft_N - image.cols,
		BORDER_CONSTANT, Scalar::all(0));

	// limpa o array de matrizes que vao compor a
	// imagem complexa
	planos.clear();
	// cria a compoente real
	realInput = Mat_<float>(padded);
	// insere as duas componentes no array de matrizes
	planos.push_back(realInput);
	planos.push_back(zeros);

	// combina o array de matrizes em uma unica
	// componente complexa
	merge(planos, complexImage);

	// calcula o dft
	dft(complexImage, complexImage);

	// realiza a troca de quadrantes
	deslocaDFT(complexImage);

	// aplica o filtro frequencial
	mulSpectrums(complexImage, filter, complexImage, 0);

	// limpa o array de planos
	planos.clear();
	// separa as partes real e imaginaria para modifica-las
	split(complexImage, planos);

	// usa o valor medio do espectro para dosar o ruido
	mean = abs(planos[0].at<float>(dft_M / 2, dft_N / 2));

	// recompoe os planos em uma unica matriz complexa
	merge(planos, complexImage);

	// troca novamente os quadrantes
	deslocaDFT(complexImage);

	cout << complexImage.size().height << endl;
	// calcula a DFT inversa
	idft(complexImage, complexImage);

	// limpa o array de planos
	planos.clear();

	// separa as partes real e imaginaria da
	// imagem filtrada
	split(complexImage, planos);

	// normaliza a parte real para exibicao
	normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
	imshow("filtrada", planos[0]);
	waitKey();

	return 0;
}