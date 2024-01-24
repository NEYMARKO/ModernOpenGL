from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from PIL import Image
import random
from datetime import datetime, timedelta

width = 800
height = 700

canSpawn = False
particle = None
particleCount = 0
texture = None
currentTime = None
positionsToDelete = []
previousX = 0
previousY = 0
particles = {}

class Texture:

    def __init__(self, texturePath):
        self.texture_id = self.load_texture(texturePath)
    def load_texture(self, texturePath):
        image = Image.open(texturePath).convert('RGBA')  # Convert to grayscale
        width, height = image.size
        image_data = image.tobytes("raw", "RGB", 0, -1)

        texture_id = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, texture_id)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     image_data)  # Corrected format to GL_RED
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

        return texture_id
class Particle:
    def __init__(self, particleLifeLength, lifeStartTime, position, velocity, id):
        self.particleLifeLength = particleLifeLength
        self.lifeStartTime = lifeStartTime
        self.velocity = velocity
        self.position = position
        #self.direction = direction
        #self.distance = 0
        self.color = 0
        self.id = id

    def moveUp(self):
        particle_life_length = timedelta(seconds=self.particleLifeLength)
        lifeLeft = 1 - (currentTime - self.lifeStartTime) / particle_life_length
        self.color = lifeLeft
        scale = 0.5 * width/height * lifeLeft
        #scale = 0.5
        glPushMatrix()
        glTranslatef(self.position[0], self.position[1], 0.0)
        glScalef(scale, scale, scale)
        self.drawParticle()
        glPopMatrix()
        self.position[0] += self.velocity[0] * 0.01
        self.position[1] += self.velocity[1] * 0.01
    def drawParticle(self):
        glColor4f(self.color, self.color, self.color, 0.0)
        glEnable(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, texture.texture_id)

        glBegin(GL_QUADS)
        glTexCoord2f(0, 0)
        glVertex3f(-1, -1, 0)

        glTexCoord2f(1, 0)
        glVertex3f(1, -1, 0)

        glTexCoord2f(1, 1)
        glVertex3f(1, 1, 0)

        glTexCoord2f(0, 1)
        glVertex3f(-1, 1, 0)
        glEnd()

    def destroy(self):
        global positionsToDelete
        particle_life_length = timedelta(seconds=self.particleLifeLength)
        if (currentTime - self.lifeStartTime > particle_life_length):
            positionsToDelete += [self.id]


def myDisplay():
    global particleCount, particles, currentTime
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    #resetira matricu transformacija => objekt se ponovno nalazi u sredistu, micu se transformacije koje su se primijenile
    glLoadIdentity()
    gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0)

    currentTime = datetime.now()
    for particle_key in particles:
        particles[particle_key].moveUp()
        particles[particle_key].destroy()

    for position in positionsToDelete:
        #print("PARTICLE AT POSITION {}: {}".format(position, particles[position]))
        #print("PARTICLE life length: ", particles[position].particleLifeLength)
        del particles[position]
    positionsToDelete.clear()
    #particle.moveUp()

    glDisable(GL_TEXTURE_2D)
    glutSwapBuffers()

def myReshape(w, h):
    global width, height
    width = w
    height = h
    glViewport(0, 0, width, height)
    #print("WIDTH: ", width)
    #print("HEIGHT: ", height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(10, width / height, 0.1, 400.0)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

def generateParticle(x, y):
    global particleCount

    directionFactor = 1
    if (x < previousX):
        directionFactor = -1
    normalizedX = ((x / width) * 2 - 1) * 17.5 * width / height  # tan(fovy/2) * udaljenost kamere - https://learnwebgl.brown37.net/_images/side_view_frustum.png
    normalizedY = (1 - (y / height) * 2) * 17.5
    # print("NORMALIZED (X,Y): ({}, {})".format(normalizedX, normalizedY))
    particleCount += 1
    randomVelocityX = (random.random() * 2 ) * directionFactor
    randomVelocityY = random.random() * 5 + 2
    randomLifeLength = random.random() * 5 + 4
    particles[particleCount] = Particle(randomLifeLength, datetime.now(), [normalizedX, normalizedY], [randomVelocityX, randomVelocityY], particleCount)


def mouseMotion(x, y):
    global previousX, previousY
    #print("CAN SPAWN: ", canSpawn)
    if (canSpawn):
        for i in range(5):
            generateParticle(x, y)
        previousX = x
        previousY = y
def mouseClick(button, state, x, y):
    global canSpawn
    #print("(X,Y): ({}, {})".format(x,y))
    if (button == GLUT_LEFT_BUTTON):
        if (state == GLUT_DOWN):
            #print("MOUSE DOWN AT POS: ({}, {})".format(x, y))
            canSpawn = True
        elif(state == GLUT_UP):
            #print("MOUSE UP")
            canSpawn = False
def animate(value):
    myDisplay()
    glutPostRedisplay()
    glutTimerFunc(10, animate, 0)

def main():
    global texture, currentTime
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(width, height)
    glutInitWindowPosition(400, 0)
    glutInit()
    window = glutCreateWindow("Particle system")
    glutReshapeFunc(myReshape)
    glutDisplayFunc(myDisplay)
    glutMouseFunc(mouseClick)
    glutMotionFunc(mouseMotion)
    glutTimerFunc(10, animate, 0)
    #particle = Particle("smoke.bmp", 10, 10)
    texture = Texture("smoke.bmp")
    currentTime = datetime.now()
    glutMainLoop()

if __name__ == '__main__':
  main()