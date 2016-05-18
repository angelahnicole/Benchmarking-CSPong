import json
import os
import errno
import argparse

# ==============================================================================================
# generate_particles.py
# ----------------------------------------------------------------------------------------------
# Angela Gross
# ----------------------------------------------------------------------------------------------
# Generates cs particle JSON files that are clones of Base.csparticle except the particles
# per emission property is changed.
# ==============================================================================================

# //////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------
# GLOBAL VARIABLES
# ---------------------------------------------------------------------------------------------

# Default particle names and base particle data holder
k_generatedParticlesFileName = "%d_particles_emitted.csparticle"
k_mainParticleFileName = "Base.csparticle"
k_mainParticleData = ""

# Read with arguments- these are filler values
k_generatedParticlesDirectory = "Generated"
k_minParticles = 0
k_maxParticles = 1000
k_particlesStep = 100

# //////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------
# HELPER FUNCTIONS
# ---------------------------------------------------------------------------------------------

def clearParticlesDirectory():
	# change to generated particles directory
	cwd = os.getcwd()
	os.chdir(k_generatedParticlesDirectory)

	# remove all .csparticle files in the directory
	filelist = [ f for f in os.listdir(".") if f.endswith(".csparticle") ]
	for f in filelist:
		os.remove(f)
		
	# go back to the current working directory
	os.chdir(cwd)

def createParticlesDirectory():
	# try to create the particles directory
	try:
		os.makedirs(k_generatedParticlesDirectory)
	except OSError as exception:
		if exception.errno != errno.EEXIST:
			raise

def createParticleFile(in_particlesEmitted):
	# copy the data and format the new file name
	newParticleData = k_mainParticleData;
	newParticleFileName = k_generatedParticlesDirectory + "/" + ( k_generatedParticlesFileName % (in_particlesEmitted) )

	# change the number of particles emitted
	newParticleData["Emitter"]["ParticlesPerEmissionProperty"] = "%s" % (in_particlesEmitted)

	# write file
	with open(newParticleFileName, 'w') as file:
		json.dump(newParticleData, file, indent=4)

# //////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------
# MAIN
# ---------------------------------------------------------------------------------------------

if __name__ == "__main__":

	# Setup argument parsing
	argParser = argparse.ArgumentParser(description='Generate Particle JSON Files')
	argParser.add_argument('-min', default=0, type=int, help='The minimum particles emitted (non-negative number, default is 0)')
	argParser.add_argument('-max', default=1000, type=int, help='The maximum particles emitted (non-negative number, default is 1000)')
	argParser.add_argument('-step', default=100, type=int, help='The step between min and max particles emitted (default is 100)')
	argParser.add_argument('-dir', default='Generated', help='The output directory name (default is "Generated" and it will create it for you)')
	args = vars(argParser.parse_args())

	# Parse arguments
	k_minParticles = args['min']
	k_maxParticles = args['max']
	k_particlesStep = args['step']
	k_generatedParticlesDirectory = args['dir']

	# Get copy of base particle file
	with open(k_mainParticleFileName, 'r') as file:
		k_mainParticleData = json.load(file)

	# Created and clear generated particles directory
	createParticlesDirectory()
	clearParticlesDirectory()

	# Generate particles 
	for i in range(k_minParticles, k_maxParticles + 1, k_particlesStep):
		createParticleFile(i)

# //////////////////////////////////////////////////////////////////////////////////////////////


