#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkN4BiasFieldCorrectionImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"




 
typedef itk::Image<float, 3>  ImageType;
typedef itk::Image<float, 3>  OutImageType;
typedef itk::ImageFileReader< ImageType > ReaderType;
typedef itk::ImageFileWriter< OutImageType > WriterType;
typedef itk::N4BiasFieldCorrectionImageFilter<ImageType, OutImageType> CorrecterType;
typedef itk::GradientAnisotropicDiffusionImageFilter< ImageType, OutImageType > Anisotropic;
typedef itk::HistogramMatchingImageFilter< ImageType, OutImageType > HistogramMatching;



int main(int argc, char * argv[])
{
  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  //Parameters
  reader1->SetFileName( argv[1] );
  reader2->SetFileName( argv[2] );
  writer->SetFileName( argv[3] );
    
  


  //Pipeline
  try
    {
      reader1->Update();
    }
  catch ( itk::ExceptionObject &err)
    {
      std::cout<<"Problems reading input image"<<std::endl;
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
    }
    
    try
    {
        reader2->Update();
    }
    catch ( itk::ExceptionObject &err)
    {
        std::cout<<"Problems reading input image"<<std::endl;
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
  
  
  //Get image specs
  ImageType::SpacingType spacing = reader1->GetOutput()->GetSpacing(); 
  ImageType::PointType origin = reader1->GetOutput()->GetOrigin(); 
  ImageType::DirectionType direction = reader1->GetOutput()->GetDirection();
  ImageType::SizeType  size = reader1->GetOutput()->GetRequestedRegion().GetSize();
  int pRow, pCol, pSli;
  pRow = size[0];
  pCol = size[1];
  pSli = size[2]; 
  ImageType::RegionType region;
  region.SetSize( size ); 
 
  
 
  CorrecterType::Pointer correcter=CorrecterType::New();
  correcter->SetInput(reader1->GetOutput());
  correcter->Update();

  Anisotropic::Pointer anisotropic = Anisotropic::New ();
  anisotropic->SetInput(correcter->GetOutput());
  anisotropic->SetNumberOfIterations(10);
  anisotropic->SetTimeStep(0.750);
  anisotropic->SetConductanceParameter(4);
  anisotropic->UseImageSpacingOn();
  anisotropic->Update();


  HistogramMatching::Pointer histogramMatching = HistogramMatching::New ();
  histogramMatching->SetInput(anisotropic->GetOutput());
  histogramMatching->SetReferenceImage( reader2->GetOutput() );
  histogramMatching->SetNumberOfHistogramLevels(160);
  histogramMatching->SetNumberOfMatchPoints(20);
  histogramMatching->Update();

  writer->SetInput(histogramMatching->GetOutput());
  
  try
    {
      writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
      std::cout<<"ExceptionObject caught !"<<std::endl;
      std::cout<< err <<std::endl;
      return EXIT_FAILURE;
    }
    
    


  return EXIT_SUCCESS;
 
  
 }