start, stop = 0, 100

def gcd(a,b):
	'''
	Hentet fra itgk oving....
	Gitt inputparametere a og b, begge heltall.
		Gjenta saa lenge b ikke er 0:
			Tilegn den navaerende verdien av b til en ny variabel gammel_b.
			Tilegn til b resten etter a dividert paa b.
			Tilegn til a verdien av gammel_b.
		Returner a som resultat av algoritmen'''
	while b > 0:
		a,b = b, a % b 
	return a 

def noCommonDividors(a,b,c):
	'''
	Returns True if biggest divider among them is 1 and they 
	'''
	#Litt stygg funksjon..?
	# could be expanded to check if 
	ab = gcd(a,b) == 1
	ac = gcd(a,c) == 1 
	bc = gcd(b,c) == 1
	return all([ab,ac,bc])
	
def is_unique(elements):
	return len(set(elements)) == len(elements)

def rightSum(a,b,c):
	return (a**2)+(b**2) == c**2
	
def isValidTrapez(a,b,c):
	return all([noCommonDividors(a,b,c),is_unique([a,b,c]), rightSum(a,b,c)])


## The funtion actually performing the trapes..
def main():
	list_of_trapezes = []
	
	for c in range(start,stop):
		for b in range(4,c):
			for a in range(3,b):
				if isValidTrapez(a,b,c):
					list_of_trapezes.append((a,b,c))
		
	for element in list_of_trapezes:
		print(element);

# Run main

main()

## TESTFUNCTIONS (pytest) just for fun...
def test_noCommonDividors_With_1_3_4_give_True():
	assert noCommonDividors(1,3,4) == True
	
def test_noCommonDividors_With_20_2_17_give_False():
	assert noCommonDividors(20,2,17) == False
	
def test_is_unique_1_2_3_should_give_True():
	assert is_unique([1,2,3]) == True
	
def test_is_unique_1_2_2_should_give_False():
	assert is_unique([1,2,2]) == False

def test_rightSum_3_4_5_give_True():
	assert rightSum(3,4,5) == True