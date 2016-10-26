package competitive.programming.physics;

import static org.junit.Assert.*;

import org.junit.Test;

import competitive.programming.geometry.Vector;
import competitive.programming.physics.Disk;

public class DiskTest {
	private Disk disk = new Disk(new Vector(2, 1), new Vector(2, 3), 5);

	@Test
	public void testMove() {
		assertEquals(new Disk(new Vector(4, 4), new Vector(2, 3), 5), disk.move());
		assertEquals(new Disk(new Vector(2, 1), new Vector(2, 3), 5), disk);
	}

	@Test
	public void testAccelerate() {
		assertEquals(new Disk(new Vector(2, 1), new Vector(3, 5), 5), disk.accelerate(new Vector(1, 2)));
		assertEquals(new Disk(new Vector(2, 1), new Vector(4, 6), 5), disk.accelerate(2));
		assertEquals(new Disk(new Vector(2, 1), new Vector(2, 3), 5), disk);
	}
	
	@Test
	public void testCollisionDetection(){
		Disk oppositeMoves = new Disk(new Vector(-2, -5), new Vector(-2, -3), 1);
		assertFalse(disk.willCollide(oppositeMoves));
		Disk frontCollision = new Disk(new Vector(6, 7), new Vector(-2, -3), 1);
		assertTrue(disk.willCollide(frontCollision));
		Disk alreadyCollide = new Disk(new Vector(2, 1), new Vector(-2, -3), 1);
		assertTrue(disk.willCollide(alreadyCollide));
		Disk noRelativeMovement = new Disk(new Vector(10, 10), new Vector(2, 3), 2);
		assertFalse(disk.willCollide(noRelativeMovement));
		Disk goingRight = new Disk(new Vector(0,0), new Vector(10, 0), 5);
		Disk radiusTouch = new Disk(new Vector(2, 6), new Vector(0, 0), 1);
		assertTrue(goingRight.willCollide(radiusTouch));
		Disk radiusNotTouch = new Disk(new Vector(2, 6), new Vector(0, 0), 0.5);
		assertFalse(goingRight.willCollide(radiusNotTouch));
	}
	
	@Test
	public void collisionTime(){
		Disk oppositeMoves = new Disk(new Vector(-2, -5), new Vector(-2, -3), 1);
		assertEquals(Double.MAX_VALUE, disk.collisionTime(oppositeMoves), 0.01);
		Disk goingRight = new Disk(new Vector(0,0), new Vector(10, 0), 5);
		Disk radiusTouch = new Disk(new Vector(2, 6), new Vector(0, 0), 1);
		assertEquals(0.2, goingRight.collisionTime(radiusTouch), 0.01);
		assertEquals(2, goingRight.collisionTime(new Disk(new Vector(26,0), new Vector(0, 0), 1)), 0.01);
		assertEquals(0, goingRight.collisionTime(goingRight), 0.01);
		assertEquals(0, goingRight.collisionTime(new Disk(new Vector(1,1), new Vector(-10, 0), 5)), 0.01);
	}

}
