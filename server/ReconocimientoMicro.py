import cv2
import socket
import time
import numpy as np
import os
from subprocess import call

def mandarCadenas(cadenaCentroideX, cadenaCentroideY, cadenaDiametro, cadenaCategoria, cl):
    #Envia las cadenas al cliente
    posicionamiento = "P-------"
    cl.send (cadenaCentroideX)
    cl.send (cadenaCentroideY)
    cl.send (cadenaCategoria)
    cl.send (cadenaDiametro)
    cl.send (posicionamiento)
    print ("Se han mandado las cadenas")

def crearString(parametro1,parametro2,tipo):
    #Transforma las coordenadas del centroide y el diametro en string
    if tipo == 'C':
        cadenaCentroideX = 'X' + str(int(parametro1))
        if len(cadenaCentroideX ) < 8:
            for i in range( 8 - len(cadenaCentroideX) ):
                cadenaCentroideX = cadenaCentroideX + '-'
        else:
            cadenaAuxiliar=''
            for i in range( 8 ):
                cadenaAuxiliar = cadenaAuxiliar + cadenaCentroideX [i]
            cadenaCentroideX = cadenaAuxiliar

        cadenaCentroideY = 'Y' + str(int(parametro2))

        if len(cadenaCentroideY ) < 8:
            for i in range( 8 - len(cadenaCentroideY) ):
                cadenaCentroideY = cadenaCentroideY + '-'
        else:
            cadenaAuxiliar=''
            for i in range( 8 ):
                cadenaAuxiliar = cadenaAuxiliar + cadenaCentroideY [i]
            cadenaCentroideY = cadenaAuxiliar
        return cadenaCentroideX, cadenaCentroideY

    elif tipo =='D':
            diametro = 'D' + str(int(parametro1))
            print (diametro)
            if len(diametro ) < 8:
                for i in range( 8 - len(diametro) ):
                    diametro = diametro + '-'
            else:
                cadenaAuxiliar=''
                for i in range( 8 ):
                    cadenaAuxiliar = cadenaAuxiliar + diametro [i]
                diametro = cadenaAuxiliar
            return diametro

def crearCadenaClasificacion(categoria):
    cadenaCategoria = 'C' + categoria[0:3]
    if len(cadenaCategoria ) < 8:
        for i in range( 8 - len(cadenaCategoria) ):
            cadenaCategoria = cadenaCategoria + '-'
    return cadenaCategoria



def procesamientoImagen(imagenInicial,factorEscalaImagen,factorEscalaRealidad,cl):
    #Filtrado de la imagen con filtro gausiano

    ########### Debug ################
    # cv2.imshow('image',imagenInicial)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    ##################################

    print ("Se realiza el filtrado de la imagen")
    #imagenInicial = cv2.GaussianBlur(imagenInicial,(5,5),0)
    #Se lleva la imagen a blanco y negro
    print ("Se pasa la imagen al dominio de la escala de grises")
    imagenGris = cv2.cvtColor(imagenInicial,cv2.COLOR_BGR2GRAY)
    #Se binariza la imagen en en (0,0,0) y (255,255,255)
    print ("Se binariza la imagen")
    ret,imagenBinaria = cv2.threshold(imagenGris,70,255,cv2.THRESH_BINARY_INV)

    ########### Debug ################
    # cv2.imshow('image',imagenBinaria)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    ##################################

    #Se hallan los contornos externos de la imagen
    print ("Se hallan lo contornos")
    im, contornos, jerarquia = cv2.findContours(imagenBinaria,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    #Se transfoma la imagen nuevamente a BGR
    imagenFinal = cv2.cvtColor(imagenBinaria,cv2.COLOR_GRAY2BGR)
    #Se dibujan los contornos
    print ("Se dibujan los contornos")
    imagenFinal = cv2.drawContours(imagenFinal, contornos,-1, (10,10,255), 2)


    ########### Debug ################
    # cv2.imshow('image',imagenFinal)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    ##################################
    #Se hallan los centroides

    call("python3 clasificarImagen.py", shell=True)
    f = open('Clasificacion/clasificado', 'r')
    categoria = f.read()
    print(categoria)
    cadenaCategoria = crearCadenaClasificacion(categoria)



    if len(contornos)==0:
        print ("No se ha encontrado centroide en la imagen")

    for i in range(len(contornos)):
        #centroides
        M = cv2.moments(contornos[i])
        if  M['m00']==0:
            print("Se encontro un parametro incorrecto, se continuara con la ejecucion")
            continue
        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])
        cv2.circle(imagenFinal, (cx, cy), 2, (0, 255, 0), thickness = 2, lineType = cv2.LINE_AA)
        #coordenadaRealX = cx * factorEscalaImagen * factorEscalaRealidad
        #coordenadaRealY = cy * factorEscalaImagen * factorEscalaRealidad
        coordenadaRealX = cx
        coordenadaRealY = cy
        print ("Centroide ", i+1," X: ", coordenadaRealX ," Y: ",coordenadaRealY)
        #Diametro equivalente
        area = cv2.contourArea(contornos[i])
        diametroEquivalente = np.sqrt(4 * area / np.pi)
        diametroReal = diametroEquivalente * factorEscalaImagen * factorEscalaRealidad
        print ("Diametro equivalente ",i + 1," = " ,diametroReal)
        #Los valores se convierten a string para poder pasarse
        cadenaCentroideX, cadenaCentroideY = crearString (coordenadaRealX, coordenadaRealY, 'C')
        cadenaDiametro = crearString(diametroReal, 0, 'D')
        #Se imprimen las cadenas obtenidas
        print (" Cadenas obtenidas ")
        print (cadenaCentroideX)
        print (cadenaCentroideY)
        print (cadenaDiametro)
        print (cadenaCategoria)
        mandarCadenas (cadenaCentroideX, cadenaCentroideY, cadenaDiametro, cadenaCategoria, cl)


    #Muestra la imagen final
    print ("Se muestra la imagen")
    #cv2.imshow('Imagen Final',imagenFinal)
    k = cv2.waitKey(3000) & 0xFF
    cv2.destroyAllWindows()


#Seleccionar entre "Video" e "Imagen". Definir factores de escala
def configuracionInicial(cl):
    #Parametros a modificar
    Selector = "Imagen"
    factorEscalaRealidad = 1
    factorEscalaImagen = 0.5
    factorCapturas = 200
    numeroCamara = 0

    if Selector == "Imagen":
        im = cv2.imread('Imagen/imagen.jpg')     # <----------------CAMBIAR
        if im is None:
            print("No se ha encontrado la imagen")
        else:
            #im = cv2.resize(im,(320, 240), interpolation = cv2.INTER_CUBIC)
            im = cv2.resize(im,(100, 100), interpolation = cv2.INTER_CUBIC)
            # os.remove('imagen.jpg')     #< -----------------CAMBIAR
            #im = cv2.resize(im, None, fx= factorEscalaImagen, fy= factorEscalaImagen, interpolation= cv2.INTER_LINEAR)
            procesamientoImagen(im,factorEscalaImagen,factorEscalaRealidad,cl)


    elif Selector == "Video":
        #Captura video de la webcam correspondiente
        video = cv2.VideoCapture( numeroCamara )
        if (video.isOpened() == False):
            print("No se ha podido leer desde la camara")
        else:
            #Se crea un contador de capturas
            capturas = 0
            while True:
                exito, im = video.read()
                if exito==True:
                    capturas = capturas + 1
                    #Se toma la imagenes cada cierta cantidad de capturas
                    if capturas == factorCapturas:
                        im = cv2.resize(im, None, fx = factorEscalaImagen, fy = factorEscalaImagen, interpolation= cv2.INTER_LINEAR)
                        cv2.imshow('Imagen tomada',im)
                        k = cv2.waitKey(1000) & 0xFF
                        procesamientoImagen(im, factorEscalaImagen, factorEscalaRealidad, cl)
                        capturas = 0
                else:
                    print (" No se ha podido capturar el video")
                if cv2.waitKey(1) & 0xFF == 27:
                    break



if __name__ == '__main__':
    pass
