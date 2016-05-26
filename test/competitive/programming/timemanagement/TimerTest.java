package competitive.programming.timemanagement;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.Test;

import competitive.programming.timemanagement.TimeoutException;
import competitive.programming.timemanagement.Timer;

public class TimerTest {

	@Test
	public void nonStartedTimerDoesNotTimeout() {
		Timer timer = new Timer();
		try {
			timer.timeCheck();
			sleep(1);
			timer.timeCheck();
		} catch (TimeoutException e) {
			fail();
		}
	}
	
	@Test
	public void doesNotTimeOutBeforeTimeoutReached() {
		Timer timer = new Timer();
		try {
			timer.startTimer(2);
			sleep(1);
			long currentTimeTaken = timer.currentTimeTakenInNanoSeconds();
			assertTrue(currentTimeTaken>1*1000*1000);
			assertTrue(currentTimeTaken<2*1000*1000);
			timer.timeCheck();
		} catch (TimeoutException e) {
			fail();
		}
	}

	@Test(expected = TimeoutException.class)
	public void timeoutReached() throws TimeoutException {
		Timer timer = new Timer();
		timer.startTimer(1);
		sleep(2);
		timer.timeCheck();
	}
	
	private void sleep(long milliseconds){
		//I had some difficulties to sleep precisely a number of milliseconds. 
		//Thread.sleep was not fine...
		long start = System.nanoTime();
		long timeout = start+milliseconds*1000*1000;
		
		while (System.nanoTime()<timeout){
			//do nothing
		}
	}
}
