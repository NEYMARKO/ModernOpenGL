from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import numpy as np


width = 800
height = 700

factor = 0
obj = None
curve = None
multiplier = 1
totalCurveColumns = int((1 - 0) / 0.01)

class Curve:
    def __init__(self, fileName):
        self.vertices = self.loadControlPoints(fileName)
        self.allCurvePoints = []
        self.allTangets = []
        self.currentVertex = 1
        self.pointOnCurveCoordinates = [0, 0, 0]

    def loadControlPoints(self, fileName):
        vertices = {}
        file = open(f"{fileName}", "r")
        lines = file.readlines()
        file.close()

        vertexCount = -1
        for line in lines:
            vertexCords = []
            line = line.strip()
            for cord in line[2:].split():
                vertexCords += [float(cord)]
            vertexCount += 1
            vertices[vertexCount] = vertexCords
        #print(vertices)
        return vertices

    def calculateAllCurvePoints(self):

        num_iterations = int((1 - 0) / 0.01)
        for i in range(self.currentVertex, len(self.vertices) - 2):
            #ide se od 0 do 1 sa skokom od 0.05
            for j in range(num_iterations + 1):
                t = 0 + j * 0.01
                self.allCurvePoints.append(self.calculatePointOnCurveCoordinates(t))
            self.currentVertex += 1
        #print("ALL CURVE COORDINATESS: ", self.allCurvePoints)
        return
    def calculatePointOnCurveCoordinates(self, t):

        cv = self.currentVertex

        Tmatrix = np.array([[t**3, t**2, t, 1]])
        Bmatrix = np.array([
            [-1, 3, -3, 1],
            [3, -6, 3, 0],
            [-3, 0, 3, 0],
            [1, 4, 1, 0]
        ])
        pointsMatrix = np.array([
            [self.vertices[cv - 1][0], self.vertices[cv - 1][1], self.vertices[cv - 1][2]],
            [self.vertices[cv][0], self.vertices[cv][1], self.vertices[cv][2]],
            [self.vertices[cv + 1][0], self.vertices[cv + 1][1], self.vertices[cv + 1][2]],
            [self.vertices[cv + 2][0], self.vertices[cv + 2][1], self.vertices[cv + 2][2]]
        ])

        #print("Tmatrix dim: {}\nBmatrix dim: {}\nPointsMatrix dim: {}".format(Tmatrix.shape, Bmatrix.shape, pointsMatrix.shape))
        result1 = np.dot(Tmatrix, Bmatrix)
        result2 = np.dot(result1, pointsMatrix)

        #result = 0.5 * Tmatrix @ Bmatrix @ pointsMatrix
        result = 1/6 * result2
        return [result[0][0], result[0][1], result[0][2]]

    def calculateAllTangents(self):
        num_iterations = int((1 - 0) / 0.01)
        for i in range(self.currentVertex, len(self.vertices) - 2):
            # ide se od 0 do 1 sa skokom od 0.05
            for j in range(num_iterations + 1):
                t = 0 + j * 0.01
                self.allTangets.append(self.calculateTangent(t))
            self.currentVertex += 1
        # print("ALL CURVE COORDINATESS: ", self.allCurvePoints)
        return
    def calculateTangent(self, t):
        cv = self.currentVertex
        Tmatrix = np.array([[t ** 2, t, 1]])
        Bmatrix = np.array([
            [-1, 3, -3, 1],
            [2, -4, 2, 0],
            [-1, 0, 1, 0],
        ])
        pointsMatrix = np.array([
            [self.vertices[cv - 1][0], self.vertices[cv - 1][1], self.vertices[cv - 1][2]],
            [self.vertices[cv][0], self.vertices[cv][1], self.vertices[cv][2]],
            [self.vertices[cv + 1][0], self.vertices[cv + 1][1], self.vertices[cv + 1][2]],
            [self.vertices[cv + 2][0], self.vertices[cv + 2][1], self.vertices[cv + 2][2]]
        ])

        # print("Tmatrix dim: {}\nBmatrix dim: {}\nPointsMatrix dim: {}".format(Tmatrix.shape, Bmatrix.shape, pointsMatrix.shape))
        result1 = np.dot(Tmatrix, Bmatrix)
        result2 = np.dot(result1, pointsMatrix)

        result = 0.5 * result2
        return [result[0][0], result[0][1], result[0][2]]
    def drawCurve(self):
        glColor3f(1.0, 0.0, 0.0)

        for i in range(0, len(self.allCurvePoints) - 1):
            glBegin(GL_LINES)
            glVertex3f(self.allCurvePoints[i][0], self.allCurvePoints[i][1], self.allCurvePoints[i][2])
            glVertex3f(self.allCurvePoints[i + 1][0], self.allCurvePoints[i + 1][1], self.allCurvePoints[i + 1][2])
            glEnd()

    def drawTangents(self, length):
        columnCount = int((1 - 0) / 0.01)
        # -1 da pocne od 0
        currentRow = curve.currentVertex - 1
        currentColumn = int(0 + columnCount * factor)
        glColor3f(0.0, 1.0, 0.0)
        currentTangentPos = currentRow * columnCount + currentColumn
        pointOnCurveX = self.allCurvePoints[currentTangentPos][0]
        pointOnCurveY = self.allCurvePoints[currentTangentPos][1]
        pointOnCurveZ = self.allCurvePoints[currentTangentPos][2]
        tangentEndX = (pointOnCurveX + self.allTangets[currentTangentPos][0]) * length
        tangentEndY = (pointOnCurveY + self.allTangets[currentTangentPos][1]) * length
        tangentEndZ = (pointOnCurveZ + self.allTangets[currentTangentPos][2]) * length
        glBegin(GL_LINES)
        glVertex3f(pointOnCurveX, pointOnCurveY, pointOnCurveZ)
        glVertex3f(tangentEndX, tangentEndY, tangentEndZ)
        glEnd()

    def drawPolygon(self):
        glColor3f(0.0, 0.0, 1.0)
        for i in range(0, len(self.vertices) - 1):
            glBegin(GL_LINES)
            glVertex3f(self.vertices[i][0], self.vertices[i][1], self.vertices[i][2])
            glVertex3f(self.vertices[i + 1][0], self.vertices[i + 1][1], self.vertices[i + 1][2])
            glEnd()

class Object:
    def __init__(self, fileName):
        self.newRotation = np.array([1.0, 1.0, 1.0])
        self.fileName = fileName.split(".txt")[0]
        self.originalOrientationAxis = self.originalOrientation()
        self.centerOfObject = [0.0, 0.0, 0.0]
        self.extremes = [10000.0, -10000.0, 10000.0, -10000.0, 10000.0, -10000.0]
        self.scalingFactors = [0.0, 0.0, 0.0]
        self.fileInfo = self.loadFileInfo(fileName)
        self.vertices = self.fileInfo[0]
        self.faces = self.fileInfo[1]
    def loadFileInfo(self, fileName):

        vertices = {}
        faces = {}

        vertexCount = 0
        faceCount = 0

        file = open(f"objectExamples/{fileName}", "r")
        lines = file.readlines()
        for line in lines:
            line.strip()
            if (line[0] == "#"):
                continue
            elif (line[0] == "v"):
                verticesList = []
                count = 0
                for vertexCoord in line[2:].split(): #ignoriraju se slovo i razmak nakon njega
                    vertexCoord = float(vertexCoord)
                    if (count == 0):
                        if (vertexCoord < self.extremes[0]):
                            self.extremes[0] = vertexCoord
                        elif (vertexCoord > self.extremes[1]):
                            self.extremes[1] = vertexCoord
                    elif (count == 1):
                        if (vertexCoord < self.extremes[2]):
                            self.extremes[2] = vertexCoord
                        elif (vertexCoord > self.extremes[3]):
                            self.extremes[3] = vertexCoord
                    else:
                        if (vertexCoord < self.extremes[4]):
                            self.extremes[4] = vertexCoord
                        elif (vertexCoord > self.extremes[5]):
                            self.extremes[5] = vertexCoord
                    verticesList += [vertexCoord]
                    count += 1
                vertices[vertexCount] = verticesList
                vertexCount += 1
            elif (line[0] == "f"):
                verticesInFace = []
                for vertex in line[2:].split():
                    verticesInFace += [int(vertex) - 1] #f 1 5 8 se odnosi na 1., 5. i 8. vrh, tj pozicije 0, 4 i 7
                faces[faceCount] = verticesInFace
                faceCount += 1
            else:
                continue
        #self.calculateCenterOfObject(vertices)
        return [vertices, faces]

    def calculateCenterOfObject(self, vertices):
        xTotalSum = 0.0
        yTotalSum = 0.0
        zTotalSum = 0.0
        for key in vertices:
            xTotalSum += vertices[key][0]
            yTotalSum += vertices[key][1]
            zTotalSum += vertices[key][2]
        self.centerOfObject[0] = xTotalSum / len(vertices)
        self.centerOfObject[1] = yTotalSum / len(vertices)
        self.centerOfObject[2] = zTotalSum / len(vertices)

    def calculateScalingFactors(self):
        self.scalingFactors[0] = 1 / (self.extremes[1] - self.extremes[0])
        self.scalingFactors[1] = 1 / (self.extremes[3] - self.extremes[2])
        self.scalingFactors[2] = 1 / (self.extremes[5] - self.extremes[4])

    def drawShape(self):
        glColor3f(1.0, 1.0, 1.0)
        for face in self.faces:

            glBegin(GL_TRIANGLE_FAN)
            glVertex3f(self.vertices[self.faces[face][0]][0], self.vertices[self.faces[face][0]][1], self.vertices[self.faces[face][0]][2])
            glVertex3f(self.vertices[self.faces[face][1]][0], self.vertices[self.faces[face][1]][1], self.vertices[self.faces[face][1]][2])
            glVertex3f(self.vertices[self.faces[face][2]][0], self.vertices[self.faces[face][2]][1], self.vertices[self.faces[face][2]][2])
            glEnd()

    def normalizeObject(self):
        self.calculateScalingFactors()
        uniformScale = max(self.scalingFactors[0], self.scalingFactors[1], self.scalingFactors[2])
        for vertex in self.vertices:
            self.vertices[vertex][0] *= uniformScale
            self.vertices[vertex][1] *= uniformScale
            self.vertices[vertex][2] *= uniformScale
        self.calculateCenterOfObject(self.vertices)
    def newObjectRotation(self, curve):

        columnCount = int((1 - 0) / 0.01)
        currentRow = curve.currentVertex - 1
        currentColumn = int(0 + columnCount * factor)
        currentTangentPos = currentRow * columnCount + currentColumn

        oa = self.originalOrientationAxis
        ta = curve.allTangets[currentTangentPos]

        rotationAxis = np.cross(oa, ta)
        rotationAxis /= np.linalg.norm(rotationAxis)

        dotProduct = np.dot(oa, ta)
        rotationAngle = np.degrees(np.arccos(dotProduct / (np.linalg.norm(oa) * np.linalg.norm(ta))))

        return rotationAxis, rotationAngle

    def originalOrientation(self):
        if (self.fileName == "F16" or self.fileName == "bird" or self.fileName == "kocka" or self.fileName == "porche"):
            return np.array([0.0, 0.0, 1.0])
        else:
            return np.array([0.0, 0.0, 1.0])
def myDisplay():
    global obj
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0.2, 0.2, 0.2, 1.0)
    #resetira matricu transformacija => objekt se ponovno nalazi u sredistu, micu se transformacije koje su se primijenile
    glLoadIdentity()
    gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0)
    glRotatef(angle, 0, 1, 0)

    rotationAxis, rotationAngle = obj.newObjectRotation(curve)
    currentCurveIndex = (curve.currentVertex - 1) * totalCurveColumns + int(0 + totalCurveColumns * factor)

    translateForward = curve.allCurvePoints[currentCurveIndex] - np.array(obj.centerOfObject)
    #print("CENTER OF OBJECT: {}".format(obj.centerOfObject))
    glPushMatrix()
    glTranslatef(translateForward[0], translateForward[1], translateForward[2])
    glRotatef(rotationAngle, rotationAxis[0], rotationAxis[1], rotationAxis[2])
    obj.drawShape()
    glPopMatrix()

    curve.drawCurve()
    curve.drawTangents(1)
    curve.drawPolygon()
    glutSwapBuffers()

def myReshape(w, h):
    global width, height
    width = w
    height = h
    glViewport(0, 0, width, height)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(10, width / height, 0.1, 400.0)
    glTranslatef(-3*width / height, -5 * width/height, 0)
    glScalef(width / height, width / height, 1)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    glClearColor(0.2, 0.2, 0.2, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glPointSize(1.0)
    glColor3f(1.0, 0.0, 0.0)

def mySpecial(key, x, y):
    global angle
    if (key == GLUT_KEY_RIGHT):
        angle += 0.5
    elif (key == GLUT_KEY_LEFT):
        angle -= 0.5

    # else:
    #     angle = 0
    glutPostRedisplay()
def animate(value):
    global angle, factor, curve, multiplier
    factor += 0.01 * multiplier
    if (factor > 1):
        factor = 0
        if (curve.currentVertex == len(curve.vertices) - 3):
            curve.currentVertex = 1
        else:
            curve.currentVertex += 1
    glutPostRedisplay()
    glutTimerFunc(10, animate, 0)

def main():
    global angle, obj, curve
    obj = Object("F16.txt")
    obj.normalizeObject()
    curve = Curve("curveInfo.txt")
    curve.calculateAllCurvePoints()
    curve.currentVertex = 1
    curve.calculateAllTangents()
    curve.currentVertex = 1
    angle = 0.0
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(width, height)
    glutInitWindowPosition(400, 0)
    glutInit()
    window = glutCreateWindow("Curve animation")
    glutReshapeFunc(myReshape)
    glutDisplayFunc(myDisplay)
    glutSpecialFunc(mySpecial)
    glutTimerFunc(10, animate, 0)
    glutMainLoop()

if __name__ == '__main__':
  main()