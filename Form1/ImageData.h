#pragma once
#include <vector>
#include <algorithm>

struct my_color {
	int R;
	int G;
	int B;
	my_color(int r, int g, int b)
	{
		this->R = r;
		this->G = g;
		this->B = b;
	}
};

void sort_it(std::vector<std::pair<my_color, int>> &sample)
{
	std::sort(sample.begin(), sample.end(), [](std::pair<my_color, int> a, std::pair<my_color, int> b) -> bool {
		return a.second < b.second;
	});
}

namespace Form1 {
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace std;

	public ref class ImageData {
	private:
		Bitmap^  _bmSource;
		Graphics^  _gHistogram;
		ProgressBar^  _pb;

		Bitmap^  _bm;
		Graphics^  _gMain;

		Color GetPixel(Byte* bPtr, int imageWidth, int x, int y)
        {
            return Color::FromArgb(*(bPtr + 3 * x + imageWidth * y),
                *(bPtr + 3 * x + imageWidth * y + 1),
                *(bPtr + 3 * x + imageWidth * y + 2));
        }

        void SetPixel(Byte* bPtr, int imageWidth, int x, int y, Color color)
        {
            *(bPtr + 3 * x + imageWidth * y) = color.R;
            *(bPtr + 3 * x + imageWidth * y + 1) = color.G;
            *(bPtr + 3 * x + imageWidth * y + 2) = color.B;
        }

	public:
		ImageData(String^  file, Graphics^  gr1, Graphics^  gr2, ProgressBar^  pb1)
		{
			_bmSource = gcnew Bitmap(file);
			_bm = gcnew Bitmap(file);
			_gMain = gr1;
			_gHistogram = gr2;
			_pb = pb1;
		}

		// Искусственное наложение шума
		void Noise()
		{
			_bm = (Bitmap^)_bmSource->Clone();
			System::Drawing::Graphics^  g = Graphics::FromImage(_bm);
			System::Drawing::Pen^  pen = gcnew Pen(Color::White, 1);
			System::Random^  rand = gcnew Random();

			for (int i = 0; i < rand->Next(50); i++)
			{
				pen->Color = Color::FromArgb(rand->Next(256), rand->Next(256), rand->Next(256));
				g->DrawRectangle(pen, rand->Next(_bm->Width), rand->Next(_bm->Height), rand->Next(100), rand->Next(100));
			}

			for (int i = 0; i < rand->Next(50); i++)
			{
				pen->Color = Color::FromArgb(rand->Next(256), rand->Next(256), rand->Next(256));
				g->DrawEllipse(pen, rand->Next(_bm->Width), rand->Next(_bm->Height), 3, 3);
			}

			for (int i = 0; i < rand->Next(50); i++)
			{
				pen->Color = Color::FromArgb(rand->Next(256), rand->Next(256), rand->Next(256));
				g->DrawLine(pen, rand->Next(_bm->Width), rand->Next(_bm->Height), rand->Next(_bm->Width), rand->Next(_bm->Height));
			}

			Draw();
		}

		// Равномерное шумоподавление
		void UniformNoiseReduction(int aperture, bool to_show)
		{
			Bitmap^  _bm_orig = (Bitmap^)_bm->Clone();

			double **h = new double*[aperture];
			for (int i = 0; i < aperture; ++i)
				h[i] = new double[aperture];

			for (int x = 0; x < aperture; x++)
			{
				for (int y = 0; y < aperture; y++)
				{
					h[x][y] = 1.0 / (aperture * aperture);
				}
			}

			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);
			System::Drawing::Imaging::BitmapData^  orig = _bm_orig->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			Byte *newBPtr_orig = (Byte*)orig->Scan0.ToPointer();
			_pb->Minimum = 0;
			_pb->Maximum = _bm->Height;
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					double sumRed = 0; double sumGreen = 0; double sumBlue = 0;
					for (double k = -aperture / 2; k <= aperture / 2; k++)
					{
						for (double l = -aperture / 2; l <= aperture / 2; l++)
						{
							Color color = GetPixel(newBPtr_orig, width, 0, 0);
							if (x + l >= 0 && x + l < _bm->Width && y + k >= 0 && y + k < _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y + k);
							else if (x + l < 0 && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 || x + l >= _bm->Width) color = GetPixel(newBPtr_orig, width, x - l, y + k);
							else if (y + k < 0 || y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y - k);

							int first = aperture / 2 + k;
							int second = aperture / 2 + l;
							sumRed += h[first][second] * color.R;
							sumGreen += h[first][second] * color.G;
							sumBlue += h[first][second] * color.B;
						}
					}
					if (sumRed > 255) sumRed = 255;
					else if (sumRed < 0) sumRed = 0;
					if (sumGreen > 255) sumGreen = 255;
					else if (sumGreen < 0) sumGreen = 0;
					if (sumBlue > 255) sumBlue = 255;
					else if (sumBlue < 0) sumBlue = 0;

					SetPixel(newBPtr, width, x, y, Color::FromArgb((Byte)sumRed, (Byte)sumGreen, (Byte)sumBlue));
				}
				_pb->Value++;
			}
			_bm->UnlockBits(modified);
			_bm_orig->UnlockBits(orig);
			_pb->Value = 0;
			if (to_show)
				Draw();
		}

		// Фильтр Гаусса
		void Gauss(int aperture, bool to_show)
		{
			Bitmap^  _bm_orig = (Bitmap^)_bm->Clone();

			double **h = new double*[aperture];
			for (int i = 0; i < aperture; ++i)
				h[i] = new double[aperture];
			const double sigma = 0.5;
			double a = 0;

			for (int x = 0; x < aperture; x++)
			{
				for (int y = 0; y < aperture; y++)
				{
					double l = x - aperture / 2;
					double k = y - aperture / 2;
					h[x][y] = Math::Exp(-(l * l + k * k) / (2 * sigma *sigma));
					a += h[x][y];
				}
			}
			a = 1.0 / a;

			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);
			System::Drawing::Imaging::BitmapData^  orig = _bm_orig->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			Byte *newBPtr_orig = (Byte*)orig->Scan0.ToPointer();
			_pb->Minimum = 0;
			_pb->Maximum = _bm->Height;
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					double sumRed = 0; double sumGreen = 0; double sumBlue = 0;
					for (double k = -aperture / 2; k <= aperture / 2; k++)
					{
						for (double l = -aperture / 2; l <= aperture / 2; l++)
						{
							Color color = GetPixel(newBPtr_orig, width, 0, 0);
							if (x + l >= 0 && x + l < _bm->Width && y + k >= 0 && y + k < _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y + k);
							else if (x + l < 0 && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 || x + l >= _bm->Width) color = GetPixel(newBPtr_orig, width, x - l, y + k);
							else if (y + k < 0 || y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y - k);

							int first = aperture / 2 + k;
							int second = aperture / 2 + l;
							sumRed += h[first][second] * color.R * a;
							sumGreen += h[first][second] * color.G * a;
							sumBlue += h[first][second] * color.B * a;
						}
					}
					if (sumRed > 255) sumRed = 255;
					else if (sumRed < 0) sumRed = 0;
					if (sumGreen > 255) sumGreen = 255;
					else if (sumGreen < 0) sumGreen = 0;
					if (sumBlue > 255) sumBlue = 255;
					else if (sumBlue < 0) sumBlue = 0;

					SetPixel(newBPtr, width, x, y, Color::FromArgb((Byte)sumRed, (Byte)sumGreen, (Byte)sumBlue));
				}
				_pb->Value++;
			}
			_bm->UnlockBits(modified);
			_bm_orig->UnlockBits(orig);
			_pb->Value = 0;
			if (to_show)
				Draw();
		}

		// Резкость
		void Focus(int aperture, int coef, bool to_clone)
		{
			if (to_clone)
				_bm = (Bitmap^)_bmSource->Clone();
			Bitmap^  _bm_orig = (Bitmap^)_bm->Clone();
			double **h = new double*[aperture];
			for (int i = 0; i < aperture; ++i)
				h[i] = new double[aperture];

			for (int x = 0; x < aperture; x++)
			{
				for (int y = 0; y < aperture; y++)
				{
					if (x != aperture / 2 || y != aperture / 2) h[x][y] = -(double)coef / (aperture * aperture - 1);
					else h[x][y] = coef + 1;
				}
			}

			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);
			System::Drawing::Imaging::BitmapData^  orig = _bm_orig->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			Byte *newBPtr_orig = (Byte*)orig->Scan0.ToPointer();
			_pb->Minimum = 0;
			_pb->Maximum = _bm->Height;
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					double sumRed = 0; double sumGreen = 0; double sumBlue = 0;
					for (double k = -aperture / 2; k <= aperture / 2; k++)
					{
						for (double l = -aperture / 2; l <= aperture / 2; l++)
						{
							Color color = GetPixel(newBPtr_orig, width, 0, 0);
							if (x + l >= 0 && x + l < _bm->Width && y + k >= 0 && y + k < _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y + k);
							else if (x + l < 0 && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 || x + l >= _bm->Width) color = GetPixel(newBPtr_orig, width, x - l, y + k);
							else if (y + k < 0 || y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y - k);

							int first = aperture / 2 + k;
							int second = aperture / 2 + l;
							sumRed += h[first][second] * color.R;
							sumGreen += h[first][second] * color.G;
							sumBlue += h[first][second] * color.B;
						}
					}
					if (sumRed > 255) sumRed = 255;
					else if (sumRed < 0) sumRed = 0;
					if (sumGreen > 255) sumGreen = 255;
					else if (sumGreen < 0) sumGreen = 0;
					if (sumBlue > 255) sumBlue = 255;
					else if (sumBlue < 0) sumBlue = 0;

					SetPixel(newBPtr, width, x, y, Color::FromArgb((Byte)sumRed, (Byte)sumGreen, (Byte)sumBlue));
				}
				_pb->Value++;
			}
			_bm->UnlockBits(modified);
			_bm_orig->UnlockBits(orig);
			_pb->Value = 0;
			Draw();
		}

		// Тиснение
		void Tisn(int aperture)
		{
			//_bm = (Bitmap^)_bmSource->Clone(); //закомментировать и вызвать Binarize_Tisn(..., false) вместо нее для другого варианта тиснения
			Bitmap^  _bm_orig = (Bitmap^)_bm->Clone();
			vector<double> h;
			for (int i = 0; i < aperture * aperture; ++i) {
				if (!i) h.push_back(1);
				else if (i == (aperture * aperture - 1)) h.push_back(-1);
				else h.push_back(0);
			}
			//другое ядро
			/*h.push_back(-1);
			h.push_back(0);
			h.push_back(1);
			h.push_back(-2);
			h.push_back(0);
			h.push_back(2);
			h.push_back(-3);
			h.push_back(0);
			h.push_back(3);*/

			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);
			System::Drawing::Imaging::BitmapData^  modified_orig = _bm_orig->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			Byte *newBPtr_orig = (Byte*)modified_orig->Scan0.ToPointer();
			_pb->Minimum = 0;
			_pb->Maximum = _bm->Height;
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					double sumRed = 0; double sumGreen = 0; double sumBlue = 0;
					for (double k = -aperture / 2; k <= aperture / 2; k++)
					{
						for (double l = -aperture / 2; l <= aperture / 2; l++)
						{
							Color color = GetPixel(newBPtr_orig, width, 0, 0);
							if (x + l >= 0 && x + l < _bm->Width && y + k >= 0 && y + k < _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y + k);
							else if (x + l < 0 && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k < 0) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x - l, y - k);
							else if (x + l < 0 || x + l >= _bm->Width) color = GetPixel(newBPtr_orig, width, x - l, y + k);
							else if (y + k < 0 || y + k >= _bm->Height) color = GetPixel(newBPtr_orig, width, x + l, y - k);

							int first = (int)(aperture / 2 + k) * aperture;
							int second = aperture / 2 + l;
							sumRed += h[first + second] * color.R;
							sumGreen += h[first + second] * color.G;
							sumBlue += h[first + second] * color.B;
						}
					}

					sumRed += 128;
					sumGreen += 128;
					sumBlue += 128;

					if (sumRed > 255) sumRed = 255;
					else if (sumRed < 0) sumRed = 0;
					if (sumGreen > 255) sumGreen = 255;
					else if (sumGreen < 0) sumGreen = 0;
					if (sumBlue > 255) sumBlue = 255;
					else if (sumBlue < 0) sumBlue = 0;

					SetPixel(newBPtr, width, x, y, Color::FromArgb((Byte)sumRed, (Byte)sumGreen, (Byte)sumBlue));
				}
				_pb->Value++;
			}
			_bm->UnlockBits(modified);
			_bm_orig->UnlockBits(modified_orig);
			_pb->Value = 0;
			Draw();
		}

		//переход к оттенкам серого / черно-белому -> тиснение
		void Binarize_Tisn(int aperture, bool to_show)
        {
			_bm = (Bitmap^)_bmSource->Clone();
			Gauss(aperture, false);
            System::Drawing::Imaging::BitmapData^  original = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::ReadOnly, PixelFormat::Format24bppRgb);
            auto width = 3 * original->Width;
            auto oldBPtr = (Byte*)original->Scan0.ToPointer();
			for (auto y = 0; y < _bm->Height; y++)
            {
                for (auto x = 0; x < _bm->Width; x++)
                {
					auto oldRed = *(oldBPtr + 3 * x + width * y);
                    auto oldGreen = *(oldBPtr + 3 * x + width * y + 1);
                    auto oldBlue = *(oldBPtr + 3 * x + width * y + 2);
                    auto bright = (Byte)(oldRed * 0.299 + oldGreen * 0.587 + oldBlue * 0.114);

                    *(oldBPtr + 3 * x + width * y) = bright;
                    *(oldBPtr + 3 * x + width * y + 1) = bright;
                    *(oldBPtr + 3 * x + width * y + 2) = bright;

					/*auto oldRed = *(oldBPtr + 3 * x + width * y);
                    auto oldGreen = *(oldBPtr + 3 * x + width * y + 1);
                    auto oldBlue = *(oldBPtr + 3 * x + width * y + 2);
                    auto bright = (Byte)(oldRed * 0.299 + oldGreen * 0.587 + oldBlue * 0.114);
                    if (bright > 127)
                    {
                        *(oldBPtr + 3 * x + width * y) = 255;
                        *(oldBPtr + 3 * x + width * y + 1) = 255;
                        *(oldBPtr + 3 * x + width * y + 2) = 255;
                    }
                    else
                    {
                        *(oldBPtr + 3 * x + width * y) = 0;
                        *(oldBPtr + 3 * x + width * y + 1) = 0;
                        *(oldBPtr + 3 * x + width * y + 2) = 0;
                    }*/
                }
            }
			if (to_show)
				Draw();
            _bm->UnlockBits(original);
			Tisn(aperture);
		}

		void Aquarel(int aperture, int coef)
		{
			_bm = (Bitmap^)_bmSource->Clone();

			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			_pb->Minimum = 0;
			_pb->Maximum = _bm->Height;
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					std::vector<std::pair<my_color, int>> colors;
					for (double k = -aperture / 2; k <= aperture / 2; k++)
					{
						for (double l = -aperture / 2; l <= aperture / 2; l++)
						{
							Color color = GetPixel(newBPtr, width, 0, 0);
							if (x + l >= 0 && x + l < _bm->Width && y + k >= 0 && y + k < _bm->Height) color = GetPixel(newBPtr, width, x + l, y + k);
							else if (x + l < 0 && y + k < 0) color = GetPixel(newBPtr, width, x - l, y - k);
							else if (x + l < 0 && y + k >= _bm->Height) color = GetPixel(newBPtr, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k < 0) color = GetPixel(newBPtr, width, x - l, y - k);
							else if (x + l >= _bm->Width && y + k >= _bm->Height) color = GetPixel(newBPtr, width, x - l, y - k);
							else if (x + l < 0 || x + l >= _bm->Width) color = GetPixel(newBPtr, width, x - l, y + k);
							else if (y + k < 0 || y + k >= _bm->Height) color = GetPixel(newBPtr, width, x + l, y - k);

							colors.push_back(std::make_pair(my_color(color.R, color.G, color.B), (Byte)(0.299 * color.R + 0.587 * color.G + 0.114 * color.B)));
						}
					}
					sort_it(colors);
					int inter = (aperture) * (aperture) / 2;
					SetPixel(newBPtr, width, x, y, Color::FromArgb((Byte)colors[inter].first.R, (Byte)colors[inter].first.G, (Byte)colors[inter].first.B));
				}
				_pb->Value++;
			}
			_bm->UnlockBits(modified);
			_pb->Value = 0;
			Focus(aperture, coef, false);
		}

		void Original()
		{
			System::Drawing::Imaging::BitmapData^  original = _bmSource->LockBits(Rectangle(0, 0, _bmSource->Width, _bmSource->Height), ImageLockMode::ReadOnly, PixelFormat::Format24bppRgb);
			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int width = 3 * original->Width;
			Byte *oldBPtr = (Byte*)original->Scan0.ToPointer();
			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();

			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					*(newBPtr + 3 * x + width * y) = *(oldBPtr + 3 * x + width * y);
					*(newBPtr + 3 * x + width * y + 1) = *(oldBPtr + 3 * x + width * y + 1);
					*(newBPtr + 3 * x + width * y + 2) = *(oldBPtr + 3 * x + width * y + 2);
				}
			}

			_bm->UnlockBits(modified);
			_bmSource->UnlockBits(original);
            Draw();
        }

		void Histogram()
        {
			System::Drawing::Imaging::BitmapData^  modified = _bm->LockBits(Rectangle(0, 0, _bm->Width, _bm->Height), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

			int *mas = new int[256];
			for (int i = 0; i < 256; ++i)
				mas[i] = 0;
			
			int width = 3 * modified->Width;

			Byte *newBPtr = (Byte*)modified->Scan0.ToPointer();
			for (int y = 0; y < _bm->Height; y++)
			{
				for (int x = 0; x < _bm->Width; x++)
				{
					int oldRed = *(newBPtr + 3 * x + width * y);
					int oldGreen = *(newBPtr + 3 * x + width * y + 1);
					int oldBlue = *(newBPtr + 3 * x + width * y + 2);
					int bright = (Byte)(0.299 * oldRed + 0.587 * oldGreen + 0.114 * oldBlue);
					mas[bright]++;
				}
			}
			_bm->UnlockBits(modified);
			_gHistogram->Clear(Color::FromKnownColor(KnownColor::Control));
			double scale = 10000.0f / (_bm->Height * _bm->Width);
			for (int i = 0; i < 256; ++i)
			{
				double y = mas[i] * scale;
				_gHistogram->DrawLine(gcnew Pen(Color::Gray, 1), i, 192, i, 192 - y);
			}
		}

		void Draw()
        {
            _gMain->Clear(Color::FromKnownColor(KnownColor::ControlDark));
            _gMain->DrawImage(_bm, 0, 0, _bm->Width, _bm->Height);
            Histogram();
        }
	};
}