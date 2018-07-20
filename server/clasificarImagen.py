# Example of a single image classification
import sys
sys.path.append('Image_Processing/')
import imageProcessing
import imageClassification

def main():
    # Classify image, the output is the category of the image
    category = imageClassification.classify("Imagen/imagen.jpg")
    f = open('Clasificacion/clasificado', 'w')
    f.write(category)
    f.close()

if __name__ == '__main__':
    main()
